# `spoken` — Corrected Implementation

> Status: implementation artifact derived from the CANONICAL spec.
>
> Source spec: `007_spoken_master_design_and_spec.md` (canonical).
> Base code:   `001_the_project_sketch.md` (the sketch).
>
> This document carries the corrected source for every file touched by the defect
> fixes (DEF-1..DEF-13) and decisions (Q-1..Q-5) in the master spec. Files not listed
> here are carried over unchanged from the sketch (see the manifest). Authoring
> compliance: `_mydocs/002_rules.md`.

---

## 1. Change Manifest

Changed files (full corrected source below):

- include/spoken/tokens.hpp        — add `lexeme` to Integer/Decimal (Q-4).
- src/tokenizer.cpp                 — populate `lexeme`; keep whitespace tokens.
- include/spoken/number_converter.hpp — add `decimal_to_words(string_view)` (Q-4).
- src/number_converter.cpp          — fix negative prefix (DEF-3); lexeme decimals (DEF-4).
- src/sentence_builder.cpp          — punctuation attaches with no space (DEF-2).
- src/conversion_pipeline.cpp       — drop Whitespace tokens (Q-1).
- include/spoken/tts/factory.hpp    — no designated-init usage at call sites (DEF-8).
- src/tts/factory.cpp               — `<iostream>`; macro-based detection (DEF-7).
- src/tts/espeak_engine.cpp         — portable temp path + POSIX gate (DEF-6).
- apps/spoken.cpp                   — parse_args without `starts_with` (DEF-1);
  serialized engine in batch (DEF-10); exit code 4 catch-all (S4); field-by-field
  EngineConfig (DEF-8); sequential sort default (Q-5).
- tests/test_number_converter.cpp   — runtime check, not static_assert (DEF-11).
- tests/test_pipeline_e2e.cpp       — remove currency test (Q-2 / DEF-5).
- tests/CMakeLists.txt              — drop currency test wiring; add factory test.

Carried over unchanged from the sketch (no defect touches them):

- include/spoken/version.hpp, include/spoken/error.hpp,
  include/spoken/tokenizer.hpp, include/spoken/symbol_converter.hpp,
  src/symbol_converter.cpp, include/spoken/sentence_builder.hpp,
  include/spoken/conversion_pipeline.hpp, include/spoken/tts/tts_engine.hpp,
  include/spoken/tts/espeak_engine.hpp, include/spoken/tts/null_engine.hpp,
  src/tts/null_engine.cpp, src/tokens.cpp, examples/batch_convert.cpp (DEF-13:
  kept as a labeled teaching artifact), tests/test_framework.hpp, tests/test_main.cpp,
  tests/test_tokenizer.cpp, tests/test_symbol_converter.cpp,
  tests/test_sentence_builder.cpp, CMakeLists.txt (already CMake >= 3.16),
  .clang-format, .clang-tidy, .github/workflows/ci.yml.

Doc-only fix DEF-12 (the incorrect "GCC 8 / inline namespace" troubleshooting note)
is corrected in the master spec, not in code.

---

## 2. `include/spoken/tokens.hpp` (Q-4)

Changes: `Integer` and `Decimal` carry the original `lexeme` so decimals render from
the exact source digits.

```cpp
#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <variant>

namespace spoken {

struct Word        { std::string  text;   };
struct Integer     { std::int64_t value;  std::string lexeme; };  // Q-4
struct Decimal     { double       value;  std::string lexeme; };  // Q-4
struct Symbol      { std::string  glyph;  };
struct Punctuation { char         mark;   };
struct Whitespace  { std::string  spaces; };

using Token =
    std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;

std::ostream& operator<<(std::ostream& os, const Token& t);

}  // namespace spoken
```

Note: `src/tokens.cpp` `operator<<` already streams `v.value`; it compiles unchanged
because the new `lexeme` member is simply not printed.

---

## 3. `src/tokenizer.cpp` (Q-4)

Changes: when emitting Integer/Decimal, store the original numeric substring as the
lexeme (separators stripped from the parsed value only).

```cpp
// In the digit branch of Tokenizer::tokenize, replace the emplace calls:
        else if (is_digit(c)) {
            std::size_t j = i;
            bool seen_dot = false;
            while (j < text.size() &&
                   (is_digit(text[j]) || text[j] == '.' || text[j] == ',')) {
                if (text[j] == '.') seen_dot = true;
                ++j;
            }
            std::string lexeme{text.substr(i, j - i)};   // original digits (Q-4)
            std::string num = lexeme;
            num.erase(std::remove(num.begin(), num.end(), ','), num.end());

            try {
                if (seen_dot) {
                    out.emplace_back(Decimal{std::stod(num), lexeme});
                } else {
                    out.emplace_back(Integer{std::stoll(num), lexeme});
                }
            } catch (const std::exception&) {
                throw InvalidInputError{"cannot parse number: " + num};
            }
            i = j;
        }
```

The rest of `tokenizer.cpp` is unchanged.

---

## 4. `include/spoken/number_converter.hpp` (Q-4)

Changes: add a lexeme-driven decimal renderer.

```cpp
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace spoken {

class NumberConverter {
public:
    [[nodiscard]] std::string to_words(std::int64_t value) const;
    [[nodiscard]] std::string to_words(double value) const;

    // Q-4 / DEF-4: render a decimal from its original lexeme, e.g.
    // "3.14" -> "three point one four". Falls back to to_words(double) if the
    // lexeme is empty.
    [[nodiscard]] std::string decimal_to_words(std::string_view lexeme) const;

private:
    static constexpr std::array<std::string_view, 20> kOnes = {
        "zero", "one", "two", "three", "four", "five", "six", "seven",
        "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
        "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
    };
    static constexpr std::array<std::string_view, 10> kTens = {
        "", "", "twenty", "thirty", "forty", "fifty",
        "sixty", "seventy", "eighty", "ninety"
    };
    static constexpr std::array<std::string_view, 7> kIllions = {
        "", "thousand", "million", "billion", "trillion",
        "quadrillion", "quintillion"
    };
};

}  // namespace spoken
```

---

## 5. `src/number_converter.cpp` (DEF-3, DEF-4)

Changes: the integer renderer builds the magnitude in a local and prepends "negative "
AFTER the chunk loop (DEF-3, which previously overwrote the prefix). Decimals are
rendered from the lexeme, digit by digit, with no float-format artifacts (DEF-4).

```cpp
#include "spoken/number_converter.hpp"

#include <cstdlib>
#include <string>

namespace spoken {
namespace {

std::string three_digit(std::uint64_t n,
                        const std::array<std::string_view, 20>& ones,
                        const std::array<std::string_view, 10>& tens) {
    std::string out;
    if (n >= 100) {
        out += std::string{ones[n / 100]};
        out += " hundred";
        n %= 100;
        if (n) out += ' ';
    }
    if (n >= 20) {
        out += std::string{tens[n / 10]};
        if (n % 10) { out += '-'; out += std::string{ones[n % 10]}; }
    } else if (n > 0) {
        out += std::string{ones[n]};
    }
    return out;
}

}  // namespace

std::string NumberConverter::to_words(std::int64_t value) const {
    if (value == 0) return std::string{kOnes[0]};

    const bool negative = value < 0;
    // Safe magnitude even for INT64_MIN.
    std::uint64_t n = negative
        ? (~static_cast<std::uint64_t>(value) + 1ULL)
        : static_cast<std::uint64_t>(value);

    std::string magnitude;                 // DEF-3: build separately
    int chunk_idx = 0;
    bool any = false;
    while (n > 0 && chunk_idx < static_cast<int>(kIllions.size())) {
        std::uint64_t chunk = n % 1000;
        if (chunk != 0) {
            std::string part = three_digit(chunk, kOnes, kTens);
            if (chunk_idx > 0) {
                part += ' ';
                part += std::string{kIllions[chunk_idx]};
            }
            if (any) { part += ' '; part += magnitude; }
            magnitude = std::move(part);
            any = true;
        }
        n /= 1000;
        ++chunk_idx;
    }

    return negative ? ("negative " + magnitude) : magnitude;   // DEF-3
}

std::string NumberConverter::decimal_to_words(std::string_view lexeme) const {
    if (lexeme.empty()) return std::string{kOnes[0]};

    bool negative = false;
    std::size_t start = 0;
    if (lexeme[0] == '-') { negative = true; start = 1; }

    const std::size_t dot = lexeme.find('.', start);
    std::string int_digits{lexeme.substr(start,
        dot == std::string_view::npos ? std::string_view::npos : dot - start)};
    int_digits.erase(std::remove(int_digits.begin(), int_digits.end(), ','),
                     int_digits.end());
    if (int_digits.empty()) int_digits = "0";

    std::string out;
    if (negative) out += "negative ";
    out += to_words(static_cast<std::int64_t>(std::stoll(int_digits)));

    if (dot != std::string_view::npos) {
        out += " point";
        for (std::size_t k = dot + 1; k < lexeme.size(); ++k) {
            const char d = lexeme[k];
            if (d >= '0' && d <= '9') {           // DEF-4: only real digits
                out += ' ';
                out += std::string{kOnes[d - '0']};
            }
        }
    }
    return out;
}

std::string NumberConverter::to_words(double value) const {
    // Retained for completeness; the pipeline uses decimal_to_words for tokens.
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%g", value);
    return decimal_to_words(buf);
}

}  // namespace spoken
```

Add `#include <algorithm>` and `#include <cstdio>` at the top alongside the existing
includes (used by `std::remove` and `std::snprintf`).

---

## 6. `src/sentence_builder.cpp` (DEF-2)

Changes: punctuation forms attach to the previous token with no leading space; words
and numbers join with a single space. Whitespace is already dropped upstream (Q-1), so
no run-collapsing is needed; ends are still trimmed.

```cpp
#include "spoken/sentence_builder.hpp"

#include <cctype>
#include <string>

namespace spoken {
namespace {

bool is_punct_form(const std::string& s) {
    if (s.size() != 1) return false;
    return std::string{".,!?;:"}.find(s[0]) != std::string::npos;
}

}  // namespace

std::string SentenceBuilder::assemble(
    const std::vector<std::string>& spoken_per_token) const {

    std::string out;
    for (const auto& form : spoken_per_token) {
        if (form.empty()) continue;                 // drop empties (incl. WS)
        if (is_punct_form(form)) {
            out += form;                            // DEF-2: no leading space
        } else {
            if (!out.empty()) out += ' ';
            out += form;
        }
    }

    // Trim ends.
    auto not_ws = [](char c){
        return !std::isspace(static_cast<unsigned char>(c));
    };
    out.erase(out.begin(),
              std::find_if(out.begin(), out.end(), not_ws));
    out.erase(std::find_if(out.rbegin(), out.rend(), not_ws).base(), out.end());

    if (!out.empty()) {
        out[0] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(out[0])));
        const char last = out.back();
        if (last != '.' && last != '?' && last != '!') out += '.';
    }
    return out;
}

}  // namespace spoken
```

Add `#include <algorithm>` for `std::find_if`.

---

## 7. `src/conversion_pipeline.cpp` (Q-1, Q-4)

Changes: Whitespace tokens map to empty (dropped by the builder); decimals render via
`decimal_to_words(lexeme)`; integers keep `to_words(value)`.

```cpp
#include "spoken/conversion_pipeline.hpp"

#include <algorithm>
#include <string>
#include <type_traits>

namespace spoken {

ConversionPipeline::ConversionPipeline() = default;

namespace {

std::string token_to_spoken(const Token& t,
                            const NumberConverter& nc,
                            const SymbolConverter& sc) {
    return std::visit([&](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, Word>) {
            return v.text;
        } else if constexpr (std::is_same_v<T, Integer>) {
            return nc.to_words(v.value);
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return nc.decimal_to_words(v.lexeme);     // Q-4 / DEF-4
        } else if constexpr (std::is_same_v<T, Symbol>) {
            return sc.to_words(v.glyph);
        } else if constexpr (std::is_same_v<T, Punctuation>) {
            return std::string{v.mark};
        } else if constexpr (std::is_same_v<T, Whitespace>) {
            return std::string{};                     // Q-1: drop whitespace
        }
    }, t);
}

}  // namespace

std::string ConversionPipeline::convert(std::string_view text) const {
    const auto tokens = spoken::tokenize(text);

    std::vector<std::string> spoken_per_token;
    spoken_per_token.reserve(tokens.size());
    std::transform(tokens.begin(), tokens.end(),
                   std::back_inserter(spoken_per_token),
                   [&](const Token& t){
                       return token_to_spoken(t, numbers_, symbols_);
                   });

    return builder_.assemble(spoken_per_token);
}

}  // namespace spoken
```

---

## 8. `src/tts/factory.cpp` (DEF-7, DEF-8)

Changes: include `<iostream>`; detect eSpeak via the compile-time
`SPOKEN_HAVE_ESPEAK` define (set by CMake) instead of a POSIX `std::system` call; the
Null-engine sink is built without designated initializers.

```cpp
#include "spoken/tts/factory.hpp"
#include "spoken/error.hpp"

#include <fstream>
#include <iostream>     // DEF-7: std::cout used below
#include <memory>
#include <ostream>

namespace spoken::tts {

namespace {
constexpr bool espeak_available() noexcept {
#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK
    return true;                       // DEF-7: cross-platform, no std::system
#else
    return false;
#endif
}
}  // namespace

std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg) {
    EngineKind kind = cfg.kind;
    if (kind == EngineKind::Auto) {
        kind = espeak_available() ? EngineKind::Espeak : EngineKind::Null;
    }

    switch (kind) {
        case EngineKind::Espeak: {
#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK
            auto eng = std::make_unique<EspeakEngine>();
            eng->set_voice(cfg.voice);
            eng->set_speed(cfg.speed_wpm);
            return eng;
#else
            throw TtsError{"eSpeak requested but not built in"};
#endif
        }
        case EngineKind::Null: {
            std::ostream* out = &std::cout;
            std::unique_ptr<std::ofstream> file;
            if (!cfg.text_sink.empty()) {
                file = std::make_unique<std::ofstream>(cfg.text_sink);
                if (!*file) throw TtsError{"cannot open " + cfg.text_sink};
                out = file.get();
            }
            auto sink = [out, f = std::move(file)](std::string_view text) {
                *out << text << '\n';
            };
            return std::make_unique<NullEngine>(std::move(sink));
        }
        case EngineKind::Auto:
            break;   // resolved above
    }
    throw TtsError{"unknown engine kind"};
}

}  // namespace spoken::tts
```

Call sites construct `EngineConfig` field-by-field (DEF-8), e.g.:

```cpp
spoken::tts::EngineConfig cfg;     // no designated initializers (DEF-8)
cfg.kind      = spoken::tts::EngineKind::Null;
cfg.voice     = "en";
cfg.speed_wpm = 175;
cfg.text_sink = "spoke.txt";
auto engine = spoken::tts::make_engine(cfg);
```

---

## 9. `src/tts/espeak_engine.cpp` — WAV path (DEF-6)

Changes: replace POSIX `mkstemp`/`/tmp` with `std::filesystem::temp_directory_path()`;
gate the CLI fallback behind a POSIX `#ifdef`; add missing includes. The default path
should call the eSpeak synthesis API (Q-3); the CLI fallback is POSIX-only (AT-1).

```cpp
// Replacement body for EspeakEngine::to_wav (other parts of the file unchanged).
#include <filesystem>
#include <fstream>
#include <cstdio>

void EspeakEngine::to_wav(std::string_view text, const std::string& wav_path) {
    if (!impl_->initialized) throw TtsError{"engine not initialized"};

    namespace fs = std::filesystem;
    const fs::path tmp = fs::temp_directory_path() / "spoken_tts_input.txt";
    {
        std::ofstream in{tmp};
        if (!in) throw TtsError{"cannot create temp input"};
        in << text;
    }

#if defined(__unix__) || defined(__APPLE__)
    // POSIX-only CLI fallback (AT-1). Production should use the synthesis API.
    const std::string cmd =
        "espeak-ng -w \"" + wav_path + "\" -f \"" + tmp.string() + "\"";
    const int rc = std::system(cmd.c_str());
    std::error_code ec;
    fs::remove(tmp, ec);
    if (rc != 0) {
        throw TtsError{"espeak-ng wav synthesis failed (rc="
                       + std::to_string(rc) + ")"};
    }
#else
    std::error_code ec;
    fs::remove(tmp, ec);
    throw TtsError{"WAV synthesis on this platform requires the eSpeak "
                   "synthesis API build (Q-3)"};
#endif
}
```

---

## 10. `apps/spoken.cpp` — corrected pieces

### 10.1 parse_args without `starts_with` (DEF-1)

```cpp
        else if (arg.rfind("-", 0) == 0) {     // DEF-1: was arg.starts_with("-")
            std::cerr << "error: unknown flag " << arg << '\n';
            return std::nullopt;
        } else {
            a.text.emplace_back(arg);
        }
```

### 10.2 EngineConfig built field-by-field (DEF-8)

```cpp
    spoken::tts::EngineConfig ec;          // DEF-8: no designated initializers
    ec.voice     = args->voice;
    ec.speed_wpm = args->speed;
    ec.kind      = args->null_tts ? spoken::tts::EngineKind::Null
                                  : spoken::tts::EngineKind::Auto;
    if (args->out && args->speak && !args->wav) {
        ec.text_sink = args->out->string();
    }
```

### 10.3 Batch: serialized engine + sequential sort (DEF-10, Q-5)

```cpp
        // Sequential sort by canonical path = deterministic order (Q-5, S7).
        std::sort(files.begin(), files.end());

        std::mutex engine_mtx;             // DEF-10: serialize engine calls
        std::atomic<std::size_t> done{0};

        std::vector<std::future<RecordResult>> futs;
        futs.reserve(files.size());
        for (const auto& f : files) {
            futs.push_back(std::async(std::launch::async,
                [&, f]() -> RecordResult {
                    auto txt = read_text_file(f);
                    if (!txt) {
                        RecordResult bad;
                        bad.source = f;
                        bad.error  = "cannot read";
                        return bad;
                    }
                    // Convert + write text in parallel; engine is serialized.
                    RecordResult r{f, {}, "", false, {}};
                    try {
                        r.spoken_text = pipe.convert(*txt);
                        const fs::path out_file = out_dir / f.filename();
                        if (!write_text_file(out_file, r.spoken_text)) {
                            r.error = "cannot write " + out_file.string();
                            return r;
                        }
                        r.written_to = out_file;
                        if (want_wav) {
                            const fs::path wav_file =
                                wav_dir / (f.filename().string() + ".wav");
                            std::lock_guard<std::mutex> lk(engine_mtx);  // DEF-10
                            engine->to_wav(r.spoken_text, wav_file.string());
                        }
                        r.ok = true;
                    } catch (const spoken::SpokenError& e) {
                        r.error = e.what();
                    }
                    return r;
                }));
        }
```

Add `#include <mutex>` to the file's includes.

### 10.4 Exit code 4 catch-all (S4)

```cpp
}
catch (const spoken::SpokenError& e) {
    std::cerr << "spoken error: " << e.what() << '\n';
    return 1;
}
catch (const std::exception& e) {
    std::cerr << "fatal: " << e.what() << '\n';
    return 1;
}
catch (...) {                              // S4: code 4 for non-standard throws
    std::cerr << "fatal: unexpected internal error\n";
    return 4;
}
```

---

## 11. Tests — corrections

### 11.1 `tests/test_number_converter.cpp` (DEF-11)

Replace the broken compile-time test (a `static_assert` on a non-`constexpr`
`std::string`) with a runtime check, and add lexeme-decimal cases:

```cpp
TEST(runtime_zero) {                       // DEF-11: was static_assert
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(0), std::string{"zero"});
}

TEST(decimals_from_lexeme) {               // DEF-4
    NumberConverter nc;
    REQUIRE_EQ(nc.decimal_to_words("3.14"),  std::string{"three point one four"});
    REQUIRE_EQ(nc.decimal_to_words("0.5"),   std::string{"zero point five"});
    REQUIRE_EQ(nc.decimal_to_words("-2.71"),
               std::string{"negative two point seven one"});
}

TEST(negatives_keep_prefix) {              // DEF-3
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(-1234),
               std::string{"negative one thousand two hundred thirty-four"});
}
```

### 11.2 `tests/test_pipeline_e2e.cpp` (Q-2 / DEF-5)

Remove `pipeline_currency` entirely; keep grammar-accurate cases:

```cpp
TEST(pipeline_basic) {
    ConversionPipeline pipe;
    REQUIRE_EQ(pipe.convert("I have 42 apples."),
               std::string{"I have forty-two apples."});
}

TEST(pipeline_symbols) {
    ConversionPipeline pipe;
    REQUIRE_EQ(pipe.convert("50% of 200 is 100"),
               std::string{"Fifty percent of two hundred is one hundred."});
}
// pipeline_currency removed (Q-2): currency fusion is Extension Exercise #2.
```

### 11.3 `tests/CMakeLists.txt`

Keep the existing list; the currency test lived inside `test_pipeline_e2e.cpp`, so no
target change is needed beyond the in-file removal. Optionally add a future
`test_factory.cpp` once the factory test (FR-11) is written.

---

## 12. Verification Checklist (defect -> evidence)

- DEF-1  -> Section 10.1 (rfind replaces starts_with).
- DEF-2  -> Section 6 (punctuation attaches without leading space).
- DEF-3  -> Section 5 (magnitude built separately, prefix preserved) + test 11.1.
- DEF-4  -> Sections 2, 4, 5, 7 (lexeme-driven decimals) + test 11.1.
- DEF-5  -> Section 11.2 (currency test removed; Q-2).
- DEF-6  -> Section 9 (temp_directory_path + POSIX gate + includes).
- DEF-7  -> Section 8 (macro detection + `<iostream>`).
- DEF-8  -> Sections 8, 10.2 (field-by-field EngineConfig).
- DEF-9  -> factory.cpp is in the manifest and CMake sources.
- DEF-10 -> Section 10.3 (mutex-serialized engine in batch).
- DEF-11 -> Section 11.1 (runtime check replaces static_assert).
- DEF-12 -> corrected in the master spec (doc-only).
- DEF-13 -> manifest (kept as labeled teaching artifact; excluded from CI gates).

Decisions verified in code: Q-1 (Section 7), Q-2 (Section 11.2), Q-3 (Section 9),
Q-4 (Sections 2-5, 7), Q-5 (Section 10.3).

---

## 13. Next Step

With the corrections specified, the remaining work is mechanical: apply these edits to
a real `spoken/` tree, then run the Phase 1-6 sequence from the master spec Section 15
(compile green -> output correct -> portability -> concurrency -> verify decisions ->
polish), gating each phase on `ctest` plus ASan/UBSan (and TSan for the batch path).
A scaffolding of the real project can be created on request.
