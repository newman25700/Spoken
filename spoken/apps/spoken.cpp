// spoken — CLI entry point (corrected per the canonical spec).
#include "spoken/conversion_pipeline.hpp"     // the text -> sentence pipeline
#include "spoken/error.hpp"                   // typed exceptions for exit-code mapping
#include "spoken/tts/factory.hpp"             // engine factory
#include "spoken/tts/tts_engine.hpp"          // engine interface
#include "spoken/version.hpp"                 // version string for help

#include <algorithm>                          // std::sort for batch ordering
#include <atomic>                             // atomic progress counter
#include <chrono>                             // batch timing
#include <filesystem>                         // paths and directory iteration
#include <fstream>                            // file IO helpers
#include <future>                             // std::async / std::future for batch parallelism
#include <iostream>                           // console IO
#include <mutex>                              // serialize engine calls in batch (DEF-10)
#include <optional>                           // optional arguments/results
#include <sstream>                            // read stdin/files into strings
#include <string>                             // text type
#include <string_view>                        // non-owning text views
#include <thread>                             // hardware-concurrency query
#include <vector>                             // collections

namespace fs = std::filesystem;               // short alias for the filesystem namespace

namespace {                                   // file-local (internal-linkage) helpers and types

// Parsed command-line options.
struct Args {
    std::vector<std::string> text;            // positional text words
    std::optional<fs::path>  file;            // --file input path
    std::optional<fs::path>  out;             // --out output path
    std::optional<fs::path>  wav;             // --wav output path
    std::optional<fs::path>  batch_dir;       // --batch directory
    std::optional<int>       jobs;            // --jobs worker count (reserved)
    std::string              voice    = "en"; // --voice (default en)
    int                      speed    = 175;  // --speed (default 175 wpm)
    bool                     speak    = true; // speak aloud unless --no-speak
    bool                     null_tts = false;// force the null engine
    bool                     verbose  = false;// diagnostics flag
    bool                     show_help = false;// --help flag
};

// Print usage to the given stream.
void print_help(std::ostream& os, const char* prog) {
    os << "spoken " << spoken::version_string << "\n"        // banner with version
       << "Usage: " << prog << " [options] [TEXT...]\n"       // usage line
       << "  -h, --help         Show this help\n"             // each option documented below
       << "  -f, --file <path>  Read input from file\n"
       << "  -o, --out <path>   Write spoken text to file (default stdout)\n"
       << "  -w, --wav <path>   Synthesize spoken text to WAV\n"
       << "      --no-speak     Do not speak aloud\n"
       << "      --voice <name> Voice (default en)\n"
       << "      --speed <wpm>  Speech rate (default 175)\n"
       << "  -j, --jobs <N>     Parallel workers for --batch\n"
       << "      --batch <dir>  Process every *.txt in <dir>\n"
       << "      --null-tts     Use null TTS (text only)\n"
       << "      --verbose      Per-record diagnostics\n";
}

// Parse argv into Args; nullopt signals an error (the caller exits 2).
[[nodiscard]] std::optional<Args> parse_args(int argc, char** argv) {
    Args a;                                   // result being filled
    for (int i = 1; i < argc; ++i) {          // iterate arguments (skip argv[0])
        const std::string_view arg{argv[i]};  // current token as a view
        auto next = [&](std::string_view name) -> std::optional<std::string> {  // fetch an option value
            if (i + 1 >= argc) {              // missing value...
                std::cerr << "error: " << name << " needs an argument\n";  // report it...
                return std::nullopt;          // and signal failure
            }
            return std::string{argv[++i]};    // consume and return the value
        };

        if (arg == "-h" || arg == "--help") {                 // help flag
            a.show_help = true;
        } else if (arg == "-f" || arg == "--file") {          // --file <path>
            if (auto v = next(arg)) a.file = *v; else return std::nullopt;
        } else if (arg == "-o" || arg == "--out") {           // --out <path>
            if (auto v = next(arg)) a.out = *v; else return std::nullopt;
        } else if (arg == "-w" || arg == "--wav") {           // --wav <path>
            if (auto v = next(arg)) a.wav = *v; else return std::nullopt;
        } else if (arg == "-j" || arg == "--jobs") {          // --jobs <N>
            if (auto v = next(arg)) a.jobs = std::stoi(*v); else return std::nullopt;
        } else if (arg == "--batch") {                        // --batch <dir>
            if (auto v = next(arg)) a.batch_dir = *v; else return std::nullopt;
        } else if (arg == "--voice") {                        // --voice <name>
            if (auto v = next(arg)) a.voice = *v; else return std::nullopt;
        } else if (arg == "--speed") {                        // --speed <wpm>
            if (auto v = next(arg)) a.speed = std::stoi(*v); else return std::nullopt;
        } else if (arg == "--no-speak") {                     // suppress audio
            a.speak = false;
        } else if (arg == "--null-tts") {                     // force null engine
            a.null_tts = true;
        } else if (arg == "--verbose") {                      // diagnostics
            a.verbose = true;
        } else if (!arg.empty() && arg[0] == '-') {           // unknown flag (DEF-1: no starts_with)
            std::cerr << "error: unknown flag " << arg << '\n';  // report it...
            return std::nullopt;                              // and fail
        } else {                                              // otherwise positional text
            a.text.emplace_back(arg);                         // collect the word
        }
    }
    return a;                                 // return the parsed options
}

// Read a whole file into a string, or nullopt on failure.
[[nodiscard]] std::optional<std::string> read_text_file(const fs::path& p) {
    std::ifstream in{p};                      // open the file
    if (!in) return std::nullopt;             // failure if it cannot open
    std::ostringstream ss;                    // buffer
    ss << in.rdbuf();                         // slurp the whole file
    return ss.str();                          // return the contents
}

// Write text to a file; return whether it succeeded.
[[nodiscard]] bool write_text_file(const fs::path& p, std::string_view text) {
    std::ofstream out{p};                     // open for writing
    if (!out) return false;                   // failure if it cannot open
    out << text;                              // write the text
    return out.good();                        // report the stream status
}

// Outcome of processing one batch record.
struct RecordResult {
    fs::path    source;                       // input file
    fs::path    written_to;                   // output file written
    std::string spoken_text;                  // the converted sentence
    bool        ok = false;                   // success flag
    std::string error;                        // error message if any
};

}  // namespace                               // close the anonymous namespace

// Function-try-block: any uncaught throw is mapped to an exit code below.
int main(int argc, char** argv) try {
    auto args = parse_args(argc, argv);       // parse the command line
    if (!args) return 2;                      // bad arguments -> exit 2
    if (args->show_help) { print_help(std::cout, argv[0]); return 0; }  // help -> exit 0

    spoken::tts::EngineConfig ec;             // engine config built field-by-field (DEF-8)
    ec.voice     = args->voice;               // set voice
    ec.speed_wpm = args->speed;               // set speed
    ec.kind      = args->null_tts ? spoken::tts::EngineKind::Null   // null if forced...
                                  : spoken::tts::EngineKind::Auto;   // else auto-detect
    if (args->out && args->speak && !args->wav) {  // text output + speak + no wav...
        ec.text_sink = args->out->string();   // route null-engine speech text to the file
    }

    std::unique_ptr<spoken::tts::TtsEngine> engine;  // owned engine
    try {                                     // guard engine construction
        engine = spoken::tts::make_engine(ec);// build it
    } catch (const spoken::TtsError& e) {     // engine init failed...
        std::cerr << "TTS init failed: " << e.what() << '\n';  // report...
        return 3;                             // exit 3
    }
    if (args->verbose) std::cerr << "engine: " << engine->name() << '\n';  // log chosen engine

    const spoken::ConversionPipeline pipe{};  // the stateless conversion pipeline

    if (args->batch_dir) {                     // ===== batch mode =====
        const fs::path in_dir  = *args->batch_dir;                 // input directory
        const fs::path out_dir = args->out.value_or(in_dir / "out");  // output dir (default <in>/out)
        const fs::path wav_dir = args->wav.value_or(fs::path{});   // optional wav dir
        const bool     want_wav = args->wav.has_value();           // whether to emit WAVs

        std::error_code ec2;                  // non-throwing filesystem error holder
        fs::create_directories(out_dir, ec2); // ensure the output dir exists
        if (want_wav) fs::create_directories(wav_dir, ec2);  // and the wav dir

        std::vector<fs::path> files;          // collected input files
        for (const auto& e : fs::directory_iterator(in_dir, ec2)) {  // iterate the directory
            if (e.is_regular_file() && e.path().extension() == ".txt") {  // pick .txt files
                files.push_back(e.path());    // collect them
            }
        }
        std::sort(files.begin(), files.end());  // deterministic order by path (Q-5/S7)

        std::mutex engine_mtx;                // serialize engine calls across workers (DEF-10)
        std::atomic<std::size_t> done{0};     // atomic progress counter
        const auto t0 = std::chrono::steady_clock::now();  // start time

        std::vector<std::future<RecordResult>> futs;  // one future per file
        futs.reserve(files.size());           // pre-size
        for (const auto& f : files) {         // launch a task per file
            futs.push_back(std::async(std::launch::async,  // force a real async task...
                [&, f]() -> RecordResult {    // task lambda: capture context by ref, path by value
                    RecordResult r{f, {}, "", false, {}};  // initialize the result
                    auto txt = read_text_file(f);          // read the file (in parallel)
                    if (!txt) { r.error = "cannot read"; return r; }  // read failure
                    try {                     // guard conversion/IO
                        r.spoken_text = pipe.convert(*txt);  // convert (thread-safe, const)
                        const fs::path of = out_dir / f.filename();  // output path
                        if (!write_text_file(of, r.spoken_text)) {   // write the spoken text...
                            r.error = "cannot write " + of.string(); // record a write error...
                            return r;          // and stop this record
                        }
                        r.written_to = of;     // record the output file
                        if (want_wav) {        // optional WAV synthesis
                            const fs::path wf =                       // compute the wav path...
                                wav_dir / (f.filename().string() + ".wav");  // <wav_dir>/<name>.wav
                            std::lock_guard<std::mutex> lk(engine_mtx);  // serialize engine (DEF-10)
                            engine->to_wav(r.spoken_text, wf.string());  // synthesize under the lock
                        }
                        r.ok = true;           // mark success
                    } catch (const spoken::SpokenError& e) {  // per-record library error...
                        r.error = e.what();    // store it (don't abort the batch)
                    }
                    return r;                  // return the record result
                }));
        }

        std::size_t ok = 0, fail = 0;         // tallies
        for (auto& fut : futs) {              // collect results in order
            auto r = fut.get();               // wait for and retrieve each result
            ++done;                           // bump the progress counter
            std::cout << "[" << done << '/' << futs.size() << "] "  // progress prefix...
                      << r.source.filename().string() << " -> "      // the source name...
                      << (r.ok ? r.written_to.string()               // output path on success...
                               : std::string{"FAILED: "} + r.error) << '\n';  // or the error
            if (r.ok) ++ok; else ++fail;      // update tallies
        }
        const auto dt = std::chrono::steady_clock::now() - t0;  // elapsed time
        std::cout << "Done: " << ok << " ok, " << fail << " failed, in "  // summary line...
                  << std::chrono::duration_cast<std::chrono::milliseconds>(dt)
                         .count()
                  << " ms.\n";
        return fail == 0 ? 0 : 1;             // exit 0 if all succeeded, else 1
    }

    std::string input;                        // ===== single-record mode =====
    if (args->file) {                         // read from --file...
        auto txt = read_text_file(*args->file);  // read it...
        if (!txt) { std::cerr << "cannot read " << *args->file << '\n'; return 1; }  // error -> exit 1
        input = std::move(*txt);              // take the contents
    } else if (!args->text.empty()) {         // else use positional words...
        for (std::size_t i = 0; i < args->text.size(); ++i) {  // join them...
            if (i) input += ' ';              // with single spaces...
            input += args->text[i];           // appending each word
        }
    } else {                                  // else read stdin...
        std::ostringstream ss;                // buffer...
        ss << std::cin.rdbuf();               // slurp stdin...
        input = ss.str();                     // take it
    }

    const std::string spoken_text = pipe.convert(input);  // convert the input

    if (args->wav) {                          // WAV requested...
        engine->to_wav(spoken_text, args->wav->string());  // synthesize to file
    } else if (args->out) {                   // else if file output...
        if (!write_text_file(*args->out, spoken_text)) {   // write it...
            std::cerr << "cannot write " << *args->out << '\n';  // error message...
            return 1;                         // exit 1 on failure
        }
    } else {                                  // else print to stdout...
        std::cout << spoken_text << '\n';     // print the sentence
    }

    if (args->speak && !args->wav && engine->name() != "null") {  // speak only with a real engine
        engine->speak(spoken_text);           // play the audio
    }
    return 0;                                 // success
}
catch (const spoken::SpokenError& e) {        // library errors...
    std::cerr << "spoken error: " << e.what() << '\n';  // report...
    return 1;                                 // exit 1
}
catch (const std::exception& e) {             // other standard exceptions...
    std::cerr << "fatal: " << e.what() << '\n';  // report...
    return 1;                                 // exit 1
}
catch (...) {                                 // any non-standard throw (S4)...
    std::cerr << "fatal: unexpected internal error\n";  // report...
    return 4;                                 // exit 4 (contract violation)
}
