#include "spoken/tts/factory.hpp"             // the factory declaration
#include "spoken/error.hpp"                   // TtsError

#include <fstream>                            // file sink for the null engine
#include <iostream>                           // std::cout default sink (DEF-7: was missing)
#include <memory>                             // std::make_unique / std::make_shared
#include <ostream>                            // stream type for the sink
#include <utility>                            // std::move

namespace spoken::tts {                       // nested TTS namespace
namespace {                                   // file-local helper

// Compile-time availability check (DEF-7: no std::system probe).
constexpr bool espeak_available() noexcept {
#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK  // when built with eSpeak...
    return true;                              // engine is available
#else                                         // otherwise...
    return false;                             // not available
#endif
}

}  // namespace                               // close the anonymous namespace

// Build the configured engine; Auto resolves to Espeak if available, else Null.
std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg) {
    EngineKind kind = cfg.kind;               // copy the requested kind
    if (kind == EngineKind::Auto) {           // resolve Auto at runtime...
        kind = espeak_available() ? EngineKind::Espeak : EngineKind::Null;  // eSpeak if present
    }

    switch (kind) {                           // build the chosen engine
        case EngineKind::Espeak: {            // --- real engine ---
#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK  // only when compiled in...
            auto eng = std::make_unique<EspeakEngine>();  // construct the engine
            eng->set_voice(cfg.voice);        // apply the voice
            eng->set_speed(cfg.speed_wpm);    // apply the speed
            return eng;                       // return it
#else                                         // when not compiled in...
            throw TtsError{"eSpeak requested but not built in"};  // fail clearly
#endif
        }
        case EngineKind::Null: {              // --- null engine ---
            std::ostream* out = &std::cout;   // default destination is stdout
            // shared_ptr so the sink lambda stays copyable (std::function needs a
            // copy-constructible target).
            std::shared_ptr<std::ofstream> file;  // optional file owner (shared)
            if (!cfg.text_sink.empty()) {     // if a file path was configured...
                file = std::make_shared<std::ofstream>(cfg.text_sink);  // open it
                if (!*file) throw TtsError{"cannot open " + cfg.text_sink};  // error on failure
                out = file.get();             // point the sink at the file stream
            }
            auto sink = [out, file](std::string_view text) {  // copyable sink capturing ptr + handle
                *out << text << '\n';         // write one line per spoken text
            };
            return std::make_unique<NullEngine>(std::move(sink));  // build the null engine
        }
        case EngineKind::Auto:                // Auto was resolved above...
            break;                            // fall through to the safety throw
    }
    throw TtsError{"unknown engine kind"};    // safety net for an unexpected kind
}

}  // namespace spoken::tts                    // close the namespace
