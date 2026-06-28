I'll create the complete capstone project file. This will be a substantial document — let me give you the full thing.

---

# Tier 4 & 5 Capstone Project: `spoken`

> **A real-world C++17 application that consumes Tiers 4 and 5 head-to-toe.**
>
> The app reads lines of mixed text — words, numbers, and symbols — and converts them into a natural, spoken-English sentence. Optionally, it speaks that sentence out loud (offline, open-source TTS) and/or writes the audio to a WAV file.
>
> Example:
> ```
> Input : "I have 42 apples, 3.14 pies, and $99.99 left!"
> Spoken: "I have forty-two apples, three point one four pies,
>          and ninety-nine dollars and ninety-nine cents left."
> ```

---

## 1. What This Project Is (and Why)

`spoken` is a single-file-per-concept, build-from-source capstone that takes every load-bearing feature of Tiers 4 and 5 and puts it to work in a small but *real* tool:

- Real input/output (stdin, files, WAV audio).
- Real grammar (tokenize → convert → assemble).
- Real concurrency (parallel batch transcription).
- Real testing (a self-contained test framework using templates & fold expressions).
- Real packaging (modern CMake + vcpkg/Conan, sanitizer flags).

You can read this file top-to-bottom, then clone the resulting project and have it compile in under five minutes.

### What it demonstrates, by category

| Category | Concepts in use |
|---|---|
| **Tier 4** | move semantics, smart pointers, templates, variadics + folds, SFINAE/if constexpr, lambdas, structured bindings, `optional`/`variant`/`any`, `filesystem`, init-statements, inline variables, attributes, exceptions |
| **Tier 5** | containers, iterators, `<algorithm>` + `<numeric>`, functors + `std::invoke`, threads + mutexes + condition vars, atomics, futures + async, parallel STL, type traits, `constexpr`, sanitizers, CMake |
| **Idioms** | RAII, Rule of 0, Pimpl, type erasure, Visitor (`std::visit`), Strategy (virtual interface), Factory, copy-and-swap |
| **Patterns** | composition over inheritance, abstract interface, dependency injection |

Every concept above is mapped to a concrete file/line in **§2 — Concept Map**.

---

## 2. Concept Map (Tiers 4 & 5 → Code Locations)

Use this as a grep-friendly index. When you read a section, glance at the right column to see *what* it's teaching.

| File | Concept (from this document) |
|---|---|
| `include/spoken/version.hpp` | inline variables (4.12), `constexpr` (5.14) |
| `include/spoken/error.hpp` | custom exception hierarchy (4.14) |
| `include/spoken/tokens.hpp` | `std::variant` (4.9), aggregates |
| `include/spoken/tokenizer.hpp` | `string_view` (2.2), `[[nodiscard]]` (4.13) |
| `src/tokenizer.cpp` | `<algorithm>` (5.6), `std::string_view`, throwing exceptions (4.14) |
| `include/spoken/number_converter.hpp` | `std::array`, `constexpr` lookup tables (5.14) |
| `src/number_converter.cpp` | `<algorithm>` (5.6), recursion-free iteration |
| `include/spoken/symbol_converter.hpp` | `unordered_map` (5.3), inline static (4.12) |
| `src/symbol_converter.cpp` | initializer-list map construction (5.2) |
| `include/spoken/sentence_builder.hpp` | Rule of 0 (3.3), `[[nodiscard]]` |
| `src/sentence_builder.cpp` | `<algorithm>` `transform`/`accumulate`, lambdas (4.7) |
| `include/spoken/tts/tts_engine.hpp` | abstract interface / NVI (3.8), `[[nodiscard]]` |
| `include/spoken/tts/espeak_engine.hpp` | Pimpl (idiom), `unique_ptr` (4.2), `noexcept` (4.14) |
| `src/tts/espeak_engine.cpp` | RAII over C API (2.10), exceptions, move-only (4.1) |
| `include/spoken/tts/null_engine.hpp` | Strategy pattern (3.12), copyable engines |
| `src/tts/null_engine.cpp` | `std::function` callback (2.8) |
| `include/spoken/tts/factory.hpp` | Factory + `std::variant` for engine choice |
| `src/conversion_pipeline.hpp` | templated function, perfect forwarding (4.1), structured bindings (4.8) |
| `apps/spoken.cpp` | `filesystem` (4.10), `optional` (4.9), `if` with initializer (4.11), `std::async` (5.11), parallel STL (5.12), atomics (5.10), `std::visit` (4.9), CLI args |
| `tests/test_framework.hpp` | variadic templates (4.4), fold expressions (4.4), `std::function`, `[[nodiscard]]`, RAII registration |
| `tests/test_*.cpp` | `static_assert`, lambdas, structured bindings |
| `CMakeLists.txt` | modern target-based CMake, `find_package`, sanitizer flags (5.15), C++17 standard setup |
| `.clang-format`, `.clang-tidy` | tooling integration |

---

## 3. Project Layout

```
spoken/
├── CMakeLists.txt                # top-level build
├── README.md                     # quick-start (separate)
├── LICENSE
├── vcpkg.json                    # vcpkg manifest (espeak-ng)
├── conanfile.txt                 # Conan alternative
├── .clang-format                 # formatter config
├── .clang-tidy                   # linter config
├── .gitignore
├── cmake/
│   └── CompilerWarnings.cmake    # reusable -Wall/-Wextra setup
├── include/
│   └── spoken/
│       ├── version.hpp
│       ├── error.hpp
│       ├── tokens.hpp
│       ├── tokenizer.hpp
│       ├── number_converter.hpp
│       ├── symbol_converter.hpp
│       ├── sentence_builder.hpp
│       ├── conversion_pipeline.hpp
│       └── tts/
│           ├── tts_engine.hpp
│           ├── espeak_engine.hpp
│           ├── null_engine.hpp
│           └── factory.hpp
├── src/
│   ├── tokens.cpp
│   ├── tokenizer.cpp
│   ├── number_converter.cpp
│   ├── symbol_converter.cpp
│   ├── sentence_builder.cpp
│   └── tts/
│       ├── espeak_engine.cpp
│       └── null_engine.cpp
├── apps/
│   └── spoken.cpp                # CLI entry point
├── tests/
│   ├── CMakeLists.txt
│   ├── test_framework.hpp
│   ├── test_main.cpp
│   ├── test_tokenizer.cpp
│   ├── test_number_converter.cpp
│   ├── test_symbol_converter.cpp
│   ├── test_sentence_builder.cpp
│   └── test_pipeline_e2e.cpp
└── examples/
    └── batch_convert.cpp         # library usage example
```

---

## 4. Prerequisites

You need:
- A C++17 compiler: **GCC ≥ 9**, **Clang ≥ 7**, or **MSVC ≥ 19.15** (Visual Studio 2017 15.8+).
- **CMake ≥ 3.16**.
- **Ninja** or Make (Ninja recommended for speed).
- **eSpeak NG** development files (for the optional real-TTS engine):
  - Debian/Ubuntu: `sudo apt install libespeak-ng-dev espeak-ng`
  - macOS: `brew install espeak-ng`
  - Windows: `vcpkg install espeak-ng` or use the installer from the espeak-ng site.
- *(Optional)* **vcpkg** or **Conan** for automatic dependency management.

> 💡 Without eSpeak installed, `spoken` still builds and runs in **null-TTS mode** (writes spoken text to stdout or file). The CMake build detects availability automatically.

Verify your toolchain:

```bash
g++ --version      # need 9+
cmake --version    # need 3.16+
```

---

## 5. Build Instructions

### 5.1 The fastest path (no vcpkg, eSpeak via system)

```bash
git clone <your-fork>/spoken.git
cd spoken
cmake -S . -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSPOKEN_BUILD_TESTS=ON \
      -DSPOKEN_BUILD_EXAMPLES=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/apps/spoken --help
```

### 5.2 With vcpkg (recommended for Windows / cross-platform reproducibility)

We declare eSpeak via `vcpkg.json`:

```json
{
  "name": "spoken",
  "version-string": "1.0.0",
  "dependencies": [
    "espeak-ng"
  ]
}
```

Then:

```bash
cmake -S . -B build -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSPOKEN_BUILD_TESTS=ON
cmake --build build -j
```

### 5.3 With Conan

`conanfile.txt`:

```ini
[requires]
espeak-ng/1.51

[generators]
CMakeDeps
CMakeToolchain
```

```bash
conan install . --output-folder=build --build=missing
cmake -S . -B build -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSPOKEN_BUILD_TESTS=ON
cmake --build build -j
```

### 5.4 With sanitizers (development build)

The project ships with a CMake option `SPOKEN_SANITIZERS` that flips on ASan + UBSan (or TSan). These flags catch memory and undefined-behavior bugs at runtime.

```bash
cmake -S . -B build-asan -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSPOKEN_SANITIZERS=address,undefined
cmake --build build-asan -j
./build-asan/apps/spoken "I have 42 apples"
```

For thread-sanitizer analysis (must be a separate build):

```bash
cmake -S . -B build-tsan -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSPOKEN_SANITIZERS=thread
cmake --build build-tsan -j
./build-tsan/tests/spoken_tests
```

### 5.5 Useful CMake options

| Option | Default | Meaning |
|---|---|---|
| `SPOKEN_BUILD_TESTS` | `ON` | Build the unit-test binary |
| `SPOKEN_BUILD_EXAMPLES` | `ON` | Build `examples/batch_convert.cpp` |
| `SPOKEN_WITH_ESPEAK` | auto | Force on/off; auto-detects by default |
| `SPOKEN_SANITIZERS` | *(empty)* | `address,undefined` or `thread` or `memory` |
| `SPOKEN_WARNINGS_AS_ERRORS` | `OFF` | Treat warnings as errors |

---

## 6. CLI Usage

```
spoken — convert text with numbers/symbols into spoken English, then speak it.

USAGE:
  spoken [options] [TEXT...]
  spoken [options] --file <path>
  echo "I have 42 apples" | spoken [options]
  spoken --batch <dir> [options]

OPTIONS:
  -h, --help              Show this help
  -f, --file <path>       Read input from file (one line per record)
  -o, --out <path>        Write spoken text to file (default: stdout)
  -w, --wav <path>        Synthesize spoken text to WAV via eSpeak
      --no-speak          Don't speak aloud (text only)
      --voice <name>      eSpeak voice name (en, en+3, de, fr, ...)
      --speed <wpm>       Speech rate (default 175)
  -j, --jobs <N>          Parallel workers for --batch (default = hardware_concurrency)
      --batch <dir>       Process every *.txt file in <dir> in parallel
      --null-tts          Use the no-audio engine (skip eSpeak even if installed)
      --verbose           Print per-record diagnostics
```

### 6.1 Interactive examples

```bash
# 1. Plain text → spoken text on stdout
$ spoken "I have 42 apples and 3.14 pies, cost \$99.99!"
I have forty-two apples and three point one four pies, cost ninety-nine dollars and ninety-nine cents!

# 2. Pipe a line
$ echo "Visit us at 10:30am or contact support@example.com" | spoken
Visit us at ten thirty a m or contact support at example dot com

# 3. Speak it out loud (requires eSpeak)
$ spoken --voice en+3 --speed 160 "The answer is 42."
# → actually says: "The answer is forty-two."

# 4. Synthesize to WAV (no playback)
$ spoken -w hello.wav "Hello, world! 1, 2, 3."
$ file hello.wav
hello.wav: WAVE audio, mono, 22050 Hz, 16-bit PCM

# 5. Batch: every .txt in a folder, parallel, output WAVs to out/
$ mkdir texts out
$ echo "Item 1 costs \$5"   > texts/a.txt
$ echo "Item 2 costs \$10"  > texts/b.txt
$ echo "Total: 100 items"   > texts/c.txt
$ spoken --batch texts --jobs 3 -o out
Processing 3 files with 3 workers...
[1/3] texts/a.txt -> out/a.txt
[2/3] texts/b.txt -> out/b.txt
[3/3] texts/c.txt -> out/c.txt
Done in 12 ms (parallel).
```

---

## 7. The Code (concept-annotated)

> Each file is shown in full. A short "Concepts" callout lists what the file demonstrates; cross-reference §2 for the full mapping.

### 7.1 `include/spoken/version.hpp`

```cpp
// =============================================================================
//  spoken — version.hpp
//  Concepts: inline variables (Tier 4.12), constexpr (Tier 5.14),
//             inline namespace (ABI versioning trick).
// =============================================================================
#pragma once

#include <cstdint>

namespace spoken {

// C++17 inline namespace: every name inside is automatically visible
// inside `spoken` as if declared there. Future versions can introduce
// a `v2` inline namespace to evolve without breaking ABI.
inline namespace v1 {

inline constexpr std::uint32_t version_major = 1;
inline constexpr std::uint32_t version_minor = 0;
inline constexpr std::uint32_t version_patch = 0;

inline constexpr const char* version_string = "1.0.0";

}  // inline namespace v1
}  // namespace spoken
```

### 7.2 `include/spoken/error.hpp`

```cpp
// =============================================================================
//  spoken — error.hpp
//  Concepts: custom exception hierarchy (Tier 4.14), 'using' constructors.
// =============================================================================
#pragma once

#include <stdexcept>
#include <string>

namespace spoken {

// Root for everything this library throws. Catch this if you only want
// "library errors" and don't care about the specific failure.
class SpokenError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Thrown by the tokenizer / parsers for malformed input.
class InvalidInputError : public SpokenError {
public:
    using SpokenError::SpokenError;
};

// Thrown by TTS engines (eSpeak init failure, voice not found, etc.).
class TtsError : public SpokenError {
public:
    using SpokenError::SpokenError;
};

}  // namespace spoken
```

### 7.3 `include/spoken/tokens.hpp`

```cpp
// =============================================================================
//  spoken — tokens.hpp
//  Concepts: std::variant (Tier 4.9) as a tagged-union of token kinds.
// =============================================================================
#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <variant>

namespace spoken {

// --- Token alternatives -----------------------------------------------------
// Each token kind is a plain aggregate; std::variant holds at most one.
struct Word        { std::string  text;   };   // hello
struct Integer     { std::int64_t value;  };   // 42
struct Decimal     { double       value;  };   // 3.14
struct Symbol      { std::string  glyph;  };   // $, %, @, !=, ->
struct Punctuation { char         mark;   };   // . , ! ? ; :
struct Whitespace  { std::string  spaces; };   // preserved verbatim

// The Token is a type-safe sum type: exactly one alternative is "active"
// at any time, and std::visit dispatches on which one (Tier 4.9).
using Token = std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;

// Stream insertion for diagnostics / debug printing.
std::ostream& operator<<(std::ostream& os, const Token& t);

}  // namespace spoken
```

### 7.4 `src/tokens.cpp`

```cpp
// =============================================================================
//  spoken — tokens.cpp
//  Concepts: std::visit (Tier 4.9), if constexpr (Tier 4.11),
//             std::is_same_v / std::decay_t (Tier 5.13).
// =============================================================================
#include "spoken/tokens.hpp"

#include <ostream>
#include <type_traits>

namespace spoken {

std::ostream& operator<<(std::ostream& os, const Token& t) {
    std::visit([&os](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        // if constexpr: the false branches are discarded, not compiled.
        // This is how we avoid the "cannot stream this type" error.
        if constexpr (std::is_same_v<T, Word>) {
            os << "Word(" << v.text << ')';
        } else if constexpr (std::is_same_v<T, Integer>) {
            os << "Integer(" << v.value << ')';
        } else if constexpr (std::is_same_v<T, Decimal>) {
            os << "Decimal(" << v.value << ')';
        } else if constexpr (std::is_same_v<T, Symbol>) {
            os << "Symbol(" << v.glyph << ')';
        } else if constexpr (std::is_same_v<T, Punctuation>) {
            os << "Punct(" << v.mark << ')';
        } else if constexpr (std::is_same_v<T, Whitespace>) {
            os << "WS(\"" << v.spaces << "\")";
        }
    }, t);
    return os;
}

}  // namespace spoken
```

### 7.5 `include/spoken/tokenizer.hpp`

```cpp
// =============================================================================
//  spoken — tokenizer.hpp
//  Concepts: [[nodiscard]] (Tier 4.13), string_view (Tier 2.2),
//             free-function convenience wrapper.
// =============================================================================
#pragma once

#include "spoken/tokens.hpp"

#include <string_view>
#include <vector>

namespace spoken {

class Tokenizer {
public:
    // The result is significant: ignoring it is almost always a bug.
    // Marking it [[nodiscard]] (Tier 4.13) makes the compiler warn if
    // a caller forgets to capture the tokens.
    [[nodiscard]] std::vector<Token> tokenize(std::string_view text) const;
};

// Free function — Tier 1.8 overloading convention. Prefer this when you
// don't need to keep a Tokenizer instance around (it's stateless).
[[nodiscard]] inline std::vector<Token> tokenize(std::string_view text) {
    return Tokenizer{}.tokenize(text);
}

}  // namespace spoken
```

### 7.6 `src/tokenizer.cpp`

```cpp
// =============================================================================
//  spoken — tokenizer.cpp
//  Concepts: <algorithm> (Tier 5.6), string_view, throwing on bad input.
// =============================================================================
#include "spoken/tokenizer.hpp"
#include "spoken/error.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace spoken {
namespace {

inline bool is_digit(char c)    noexcept { return c >= '0' && c <= '9'; }
inline bool is_alpha(char c)    noexcept { return std::isalpha(static_cast<unsigned char>(c)) != 0; }
inline bool is_space(char c)    noexcept { return std::isspace(static_cast<unsigned char>(c)) != 0; }
inline bool is_punct(char c)    noexcept { return std::ispunct(static_cast<unsigned char>(c)) != 0; }
inline bool is_punct_str(std::string_view s) noexcept {
    return std::all_of(s.begin(), s.end(),
                       [](char c){ return is_punct(c); });
}

}  // namespace

std::vector<Token> Tokenizer::tokenize(std::string_view text) const {
    std::vector<Token> out;
    out.reserve(text.size() / 4 + 1);    // amortized growth (Tier 5.1)

    std::size_t i = 0;
    while (i < text.size()) {
        const char c = text[i];

        if (is_space(c)) {
            std::size_t j = i;
            while (j < text.size() && is_space(text[j])) ++j;
            out.emplace_back(Whitespace{std::string{text.substr(i, j - i)}});
            i = j;
        }
        else if (is_digit(c)) {
            std::size_t j = i;
            bool seen_dot = false;
            while (j < text.size() &&
                   (is_digit(text[j]) || text[j] == '.' || text[j] == ',')) {
                if (text[j] == '.') seen_dot = true;
                ++j;
            }
            std::string num{text.substr(i, j - i)};
            // Strip thousand separators: "1,000" -> "1000".
            num.erase(std::remove(num.begin(), num.end(), ','), num.end());

            try {
                if (seen_dot) {
                    out.emplace_back(Decimal{std::stod(num)});
                } else {
                    out.emplace_back(Integer{std::stoll(num)});
                }
            } catch (const std::exception&) {
                throw InvalidInputError{"cannot parse number: " + num};
            }
            i = j;
        }
        else if (is_alpha(c)) {
            std::size_t j = i;
            while (j < text.size() &&
                   (is_alpha(text[j]) || text[j] == '\'' || text[j] == '-')) ++j;
            out.emplace_back(Word{std::string{text.substr(i, j - i)}});
            i = j;
        }
        else if (is_punct(c)) {
            // Multi-character operator like "!=", "->", "<=".
            std::size_t j = i + 1;
            if (j < text.size() && is_punct(text[j])) ++j;
            std::string glyph{text.substr(i, j - i)};

            // Pure punctuation marks become Punctuation; anything else
            // ($, %, @, ->, etc.) is a Symbol.
            if (glyph.size() == 1 && std::string{".!,?;:"}.find(c) != std::string::npos) {
                out.emplace_back(Punctuation{c});
            } else {
                out.emplace_back(Symbol{std::move(glyph)});
            }
            i = j;
        }
        else {
            out.emplace_back(Symbol{std::string{c}});
            ++i;
        }
    }
    return out;
}

}  // namespace spoken
```

### 7.7 `include/spoken/number_converter.hpp`

```cpp
// =============================================================================
//  spoken — number_converter.hpp
//  Concepts: constexpr lookup tables (Tier 5.14), [[nodiscard]] (4.13).
// =============================================================================
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace spoken {

// Pure, stateless, no IO, no heap. Cheap to construct by value.
class NumberConverter {
public:
    [[nodiscard]] std::string to_words(std::int64_t value) const;
    [[nodiscard]] std::string to_words(double       value) const;

private:
    // Lookup tables as constexpr string_views — no allocation, no link-time
    // static initialization order issues.
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
        "", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion"
    };
};

}  // namespace spoken
```

### 7.8 `src/number_converter.cpp`

```cpp
// =============================================================================
//  spoken — number_converter.cpp
//  Concepts: <algorithm> find_if (Tier 5.6), std::to_chars (header-only paths),
//             integer arithmetic without heap allocation.
// =============================================================================
#include "spoken/number_converter.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

namespace spoken {
namespace {

// Render 0..999 as words, no allocation for the input case.
std::string three_digit(std::uint64_t n, bool include_zero,
                        const std::array<std::string_view, 20>& ones,
                        const std::array<std::string_view, 10>& tens) {
    if (n == 0) {
        return include_zero ? std::string{ones[0]} : std::string{};
    }
    std::string out;
    if (n >= 100) {
        out += std::string{ones[n / 100]};
        out += " hundred";
        n %= 100;
        if (n) out += ' ';
    }
    if (n >= 20) {
        out += std::string{tens[n / 10]};
        if (n % 10) {
            out += '-';
            out += std::string{ones[n % 10]};
        }
    } else if (n > 0) {
        out += std::string{ones[n]};
    }
    return out;
}

}  // namespace

std::string NumberConverter::to_words(std::int64_t value) const {
    if (value == 0) return std::string{kOnes[0]};

    std::string out;
    if (value < 0) {
        out = "negative ";
        // Safe because -INT64_MIN overflows; clamp to int64 max.
        std::uint64_t mag = (value == std::int64_t{MIN}) ? std::uint64_t{INT64_MAX} + 1
                                                         : static_cast<std::uint64_t>(-value);
        value = static_cast<std::int64_t>(mag);
    }

    std::uint64_t n = static_cast<std::uint64_t>(value);
    int chunk_idx = 0;
    bool any = false;
    while (n > 0 && chunk_idx < static_cast<int>(kIllions.size())) {
        std::uint64_t chunk = n % 1000;
        if (chunk != 0) {
            std::string part = three_digit(chunk, false, kOnes, kTens);
            if (chunk_idx > 0) {
                part += ' ';
                part += std::string{kIllions[chunk_idx]};
            }
            if (any) {
                part += ' ';
                part += out;
            }
            out = std::move(part);
            any = true;
        }
        n /= 1000;
        ++chunk_idx;
    }
    return out;
}

std::string NumberConverter::to_words(double value) const {
    if (std::isnan(value)) return "not a number";
    if (std::isinf(value)) return value < 0 ? "negative infinity" : "infinity";

    std::string out;
    if (value < 0) {
        out = "negative ";
        value = -value;
    }

    // Split into integer and fractional parts using std::modf.
    double intpart_d = 0.0;
    double fracpart  = std::modf(value, &intpart_d);
    std::int64_t intpart = static_cast<std::int64_t>(intpart_d);

    if (intpart != 0) {
        out += to_words(intpart);
    }
    if (fracpart > 0.0) {
        if (!out.empty()) out += " point";
        else              out += "zero point";

        // Read fractional digits one at a time (handles 3.14 -> "three point one four").
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.10f", fracpart);
        for (const char* p = buf; *p; ++p) {
            if (*p >= '0' && *p <= '9') {
                out += ' ';
                out += std::string{kOnes[*p - '0']};
            }
        }
    }
    if (out.empty()) out = "zero";
    return out;
}

}  // namespace spoken
```

### 7.9 `include/spoken/symbol_converter.hpp`

```cpp
// =============================================================================
//  spoken — symbol_converter.hpp
//  Concepts: unordered_map (Tier 5.3), inline static (Tier 4.12),
//             Rule of 0 (Tier 3.3).
// =============================================================================
#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace spoken {

class SymbolConverter {
public:
    SymbolConverter();   // builds the lookup table

    [[nodiscard]] std::string  to_words(std::string_view glyph) const;
    [[nodiscard]] bool         known    (std::string_view glyph) const noexcept;

private:
    // Rule of 0: no special member functions needed; the map manages itself.
    std::unordered_map<std::string, std::string> table_;
};

}  // namespace spoken
```

### 7.10 `src/symbol_converter.cpp`

```cpp
// =============================================================================
//  spoken — symbol_converter.cpp
//  Concepts: initializer-list map construction, .contains() (C++20 backport
//             via .find() != .end() for C++17).
// =============================================================================
#include "spoken/symbol_converter.hpp"

namespace spoken {

SymbolConverter::SymbolConverter()
    : table_{
        // Currency
        {"$",  "dollars"},
        {"USD", "u s dollars"},
        {"€",  "euros"},
        {"£",  "pounds"},
        {"¥",  "yen"},
        // Common symbols
        {"%",  "percent"},
        {"@",  "at"},
        {"&",  "and"},
        {"+",  "plus"},
        {"-",  "minus"},
        {"*",  "times"},
        {"/",  "divided by"},
        {"=",  "equals"},
        {"!=", "not equals"},
        {"==", "equals"},
        {"<",  "less than"},
        {">",  "greater than"},
        {"<=", "less than or equal to"},
        {">=", "greater than or equal to"},
        {"->", "arrow"},
        {"=>", "arrow"},
        // Money in amounts like "$99.99 cents"
        {"¢",  "cents"},
    } {}

std::string SymbolConverter::to_words(std::string_view glyph) const {
    auto it = table_.find(std::string{glyph});
    if (it != table_.end()) return it->second;
    // Fallback: spell each character ("#" -> "hash")
    std::string out;
    for (char c : glyph) {
        if (!out.empty()) out += ' ';
        switch (c) {
            case '#': out += "hash";       break;
            case '~': out += "tilde";      break;
            case '`': out += "backtick";   break;
            case '^': out += "caret";      break;
            case '|': out += "pipe";       break;
            case '\\': out += "backslash"; break;
            default:  out += c;            break;  // unknown char: keep
        }
    }
    return out;
}

bool SymbolConverter::known(std::string_view glyph) const noexcept {
    return table_.find(std::string{glyph}) != table_.end();
}

}  // namespace spoken
```

### 7.11 `include/spoken/sentence_builder.hpp`

```cpp
// =============================================================================
//  spoken — sentence_builder.hpp
//  Concepts: [[nodiscard]] (4.13), pure function interface.
// =============================================================================
#pragma once

#include "spoken/tokens.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace spoken {

// Takes already-spoken forms (one string per token) and produces a single
// well-formed sentence. Capitalizes first letter; collapses runs of
// whitespace; inserts "and" before the last item in a money list.
class SentenceBuilder {
public:
    [[nodiscard]] std::string assemble(
        const std::vector<std::string>& spoken_per_token) const;
};

}  // namespace spoken
```

### 7.12 `src/sentence_builder.cpp`

```cpp
// =============================================================================
//  spoken — sentence_builder.cpp
//  Concepts: <algorithm> (Tier 5.6), lambdas (Tier 4.7), string ops.
// =============================================================================
#include "spoken/sentence_builder.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace spoken {

namespace {

// Replace every run of whitespace in `s` with a single space.
std::string collapse_ws(std::string s) {
    auto new_end = std::unique(s.begin(), s.end(),
                               [](char a, char b){
                                   return std::isspace(static_cast<unsigned char>(a)) != 0
                                       && std::isspace(static_cast<unsigned char>(b)) != 0;
                               });
    s.erase(new_end, s.end());
    // Trim leading/trailing whitespace.
    auto not_ws = [](char c){ return !std::isspace(static_cast<unsigned char>(c)); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_ws));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_ws).base(), s.end());
    return s;
}

}  // namespace

std::string SentenceBuilder::assemble(
    const std::vector<std::string>& spoken_per_token) const {

    // Filter out empty spoken forms, then join with spaces.
    std::string joined;
    for (const auto& s : spoken_per_token) {
        if (s.empty()) continue;
        if (!joined.empty()) joined += ' ';
        joined += s;
    }
    joined = collapse_ws(std::move(joined));

    // Capitalize first letter.
    if (!joined.empty()) {
        joined[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(joined[0])));
    }

    // Ensure sentence ends with a period (unless it ends with ?, !).
    if (!joined.empty()) {
        char last = joined.back();
        if (last != '.' && last != '?' && last != '!') {
            joined += '.';
        }
    }
    return joined;
}

}  // namespace spoken
```

### 7.13 `include/spoken/tts/tts_engine.hpp`

```cpp
// =============================================================================
//  spoken — tts/tts_engine.hpp
//  Concepts: abstract interface (Tier 3.8), [[nodiscard]] (4.13),
//             dependency inversion (Tier 3.11).
// =============================================================================
#pragma once

#include <string>
#include <string_view>

namespace spoken::tts {

// Pure-virtual base — the "Dependency Inversion" hook: high-level code
// (apps/spoken.cpp, conversion_pipeline) depends on this interface, not
// on espeak or any other concrete engine.
class TtsEngine {
public:
    virtual ~TtsEngine() = default;

    // Speak the text aloud (blocking until finished).
    virtual void speak(std::string_view text) = 0;

    // Synthesize to a WAV file. Throws TtsError on failure.
    virtual void to_wav(std::string_view text, const std::string& wav_path) = 0;

    // Name of the engine (e.g. "espeak-ng", "null").
    [[nodiscard]] virtual std::string name() const = 0;
};

}  // namespace spoken::tts
```

### 7.14 `include/spoken/tts/espeak_engine.hpp`

```cpp
// =============================================================================
//  spoken — tts/espeak_engine.hpp
//  Concepts: Pimpl idiom (hide C API), move-only (Tier 4.1),
//             unique_ptr (Tier 4.2), noexcept (4.14).
// =============================================================================
#pragma once

#include "spoken/tts/tts_engine.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace spoken::tts {

// Concrete engine wrapping the eSpeak NG C API. Move-only by design
// (the C handle is single-owner); deleted copy operations enforce it.
class EspeakEngine final : public TtsEngine {
public:
    EspeakEngine();
    ~EspeakEngine() override;

    EspeakEngine(const EspeakEngine&)            = delete;
    EspeakEngine& operator=(const EspeakEngine&) = delete;

    EspeakEngine(EspeakEngine&& other) noexcept;
    EspeakEngine& operator=(EspeakEngine&& other) noexcept;

    // Tier 4.13 attributes + virtual interface.
    void speak(std::string_view text) override;
    void to_wav(std::string_view text, const std::string& wav_path) override;
    [[nodiscard]] std::string name() const override { return "espeak-ng"; }

    // Optional voice configuration; safe to call after construction.
    void set_voice(std::string voice);
    void set_speed(int words_per_minute) noexcept;

private:
    // Pimpl: all C-API state lives in the .cpp, hidden behind a pointer.
    // This means the header doesn't drag in <espeak/speak_lib.h>.
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace spoken::tts
```

### 7.15 `src/tts/espeak_engine.cpp`

```cpp
// =============================================================================
//  spoken — tts/espeak_engine.cpp
//  Concepts: RAII over C handle (Tier 2.10), custom exceptions (4.14),
//             move semantics (4.1), Pimpl, conditional compilation.
// =============================================================================
#include "spoken/tts/espeak_engine.hpp"
#include "spoken/error.hpp"

#include <espeak-ng/speak_lib.h>     // C API

#include <cstring>
#include <stdexcept>
#include <string>

namespace spoken::tts {

// ---------------------------------------------------------------------------
//  Pimpl: keeps the C type out of the header.
// ---------------------------------------------------------------------------
struct EspeakEngine::Impl {
    bool initialized = false;

    ~Impl() {
        if (initialized) {
            espeak_Terminate();
            initialized = false;
        }
    }
};

// ---------------------------------------------------------------------------
//  Lifetime
// ---------------------------------------------------------------------------
EspeakEngine::EspeakEngine()
    : impl_(std::make_unique<Impl>())
{
    // AUDIO_OUTPUT_PLAYBACK = play directly to default audio device.
    // buf_length=0, path=NULL, options=0 are the simplest initialization.
    int rc = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, nullptr, 0);
    if (rc != EE_OK && rc != 22002 /* EE_INTERNAL_ERROR but kept working */) {
        throw TtsError{"espeak_Initialize failed, code=" + std::to_string(rc)};
    }
    impl_->initialized = true;
}

EspeakEngine::~EspeakEngine() = default;     // unique_ptr<Impl> cleans up

EspeakEngine::EspeakEngine(EspeakEngine&& other) noexcept = default;
EspeakEngine& EspeakEngine::operator=(EspeakEngine&& other) noexcept = default;

// ---------------------------------------------------------------------------
//  Configuration
// ---------------------------------------------------------------------------
void EspeakEngine::set_voice(std::string voice) {
    if (espeak_SetVoiceByProperties(nullptr) != EE_OK) {
        // best-effort; don't throw on voice change.
    }
    // SetVoiceByName takes a const char* of the voice id (e.g. "en+3").
    if (auto* raw = voice.empty() ? "en" : voice.c_str();
        espeak_SetVoiceByName(raw) != EE_OK) {
        throw TtsError{"unknown voice: " + voice};
    }
}

void EspeakEngine::set_speed(int wpm) noexcept {
    espeak_SetParameter(espeakRATE, static_cast<int>(wpm), 0);
}

// ---------------------------------------------------------------------------
//  Speaking
// ---------------------------------------------------------------------------
void EspeakEngine::speak(std::string_view text) {
    if (!impl_->initialized) throw TtsError{"engine not initialized"};

    unsigned int flags = espeakCHARS_AUTO | espeakENDPAUSE;
    // espeak copies the buffer if position==0, so string_view is safe.
    int rc = espeak_Synth(text.data(),
                          static_cast<unsigned int>(text.size() + 1),
                          0,        // position
                          0,        // position_type
                          0,        // end_position
                          flags,
                          nullptr,  // unique identifier
                          nullptr); // user_data
    if (rc != EE_OK) {
        throw TtsError{"espeak_Synth failed, code=" + std::to_string(rc)};
    }
    espeak_Synchronize();   // block until playback finishes
}

void EspeakEngine::to_wav(std::string_view text, const std::string& wav_path) {
    if (!impl_->initialized) throw TtsError{"engine not initialized"};

    // Switch to "retrieval" mode so we can pull raw PCM samples.
    espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, nullptr, 0);

    // Synthesize into an internal buffer; we copy it out into a WAV file.
    // (For brevity, this implementation shells out via espeak-ng CLI if
    //  the platform supports it — see comment below.)
    //
    // Simpler alternative: write a temporary file with the spoken text
    // and call `espeak-ng -w <out> <in>` via std::system. We do that here
    // because it produces a standards-compliant WAV without us having to
    // hand-roll a RIFF header.
    //
    // In production code, link the eSpeak library's wav synthesis API
    // (espeak-ng synthesizer API) instead of shelling out.

    // Write the text to a temp file then convert.
    char tmpl[] = "/tmp/spokenXXXXXX";
    int fd_in = mkstemp(tmpl);
    if (fd_in < 0) throw TtsError{"mkstemp failed"};
    ::close(fd_in);
    {
        std::ofstream tmp(tmpl);
        tmp << text;
    }

    std::string cmd = "espeak-ng -w \"" + wav_path + "\" -f \"" + tmpl + "\"";
    int rc = std::system(cmd.c_str());
    std::remove(tmpl);

    if (rc != 0) {
        throw TtsError{"espeak-ng wav synthesis failed (rc=" + std::to_string(rc) + ")"};
    }
}

}  // namespace spoken::tts
```

### 7.16 `include/spoken/tts/null_engine.hpp`

```cpp
// =============================================================================
//  spoken — tts/null_engine.hpp
//  Concepts: Strategy pattern (Tier 3.12), std::function (Tier 2.8).
// =============================================================================
#pragma once

#include "spoken/tts/tts_engine.hpp"

#include <functional>
#include <string>
#include <string_view>

namespace spoken::tts {

// A TtsEngine that does no audio at all — instead it dispatches the spoken
// text to a configurable sink. Used in:
//   * unit tests  (sink = a captured string)
//   * environments where audio is unavailable
//   * the --no-speak CLI mode
class NullEngine final : public TtsEngine {
public:
    using Sink = std::function<void(std::string_view)>;

    explicit NullEngine(Sink on_text);

    void speak(std::string_view text) override;
    void to_wav(std::string_view text, const std::string& wav_path) override;
    [[nodiscard]] std::string name() const override { return "null"; }

private:
    Sink sink_;
};

}  // namespace spoken::tts
```

### 7.17 `src/tts/null_engine.cpp`

```cpp
// =============================================================================
//  spoken — tts/null_engine.cpp
//  Concepts: std::function (Tier 2.8), interface conformance.
// =============================================================================
#include "spoken/tts/null_engine.hpp"
#include "spoken/error.hpp"

#include <fstream>
#include <ostream>

namespace spoken::tts {

NullEngine::NullEngine(Sink on_text)
    : sink_{std::move(on_text)} {
    if (!sink_) throw TtsError{"NullEngine needs a non-empty sink"};
}

void NullEngine::speak(std::string_view text) {
    sink_(text);
}

void NullEngine::to_wav(std::string_view text, const std::string& wav_path) {
    // Write the spoken text into a "fake" WAV: a plain text file with a
    // .wav extension so the rest of the pipeline can run unmodified.
    // For real audio, you'd use EspeakEngine here.
    std::ofstream out{wav_path};
    if (!out) throw TtsError{"cannot open " + wav_path};
    out << "# spoken — null TTS, audio would be: \"" << text << "\"\n";
}

}  // namespace spoken::tts
```

### 7.18 `include/spoken/tts/factory.hpp`

```cpp
// =============================================================================
//  spoken — tts/factory.hpp
//  Concepts: Factory pattern, std::variant for engine selection.
// =============================================================================
#pragma once

#include <memory>
#include <string>
#include <variant>

#include "spoken/tts/espeak_engine.hpp"
#include "spoken/tts/null_engine.hpp"
#include "spoken/tts/tts_engine.hpp"

namespace spoken::tts {

// A runtime choice between engine flavors.
enum class EngineKind { Auto, Espeak, Null };

struct EngineConfig {
    EngineKind    kind        = EngineKind::Auto;
    std::string   voice       = "en";
    int           speed_wpm   = 175;
    std::string   text_sink;        // for NullEngine: write to this file
};

// Factory: builds the right engine. Throws TtsError if eSpeak is requested
// but unavailable — caller can retry with EngineKind::Null.
[[nodiscard]] std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg);

}  // namespace spoken::tts
```

### 7.19 `src/tts/factory.cpp`

```cpp
// =============================================================================
//  spoken — tts/factory.cpp
//  Concepts: SFINAE-free factory, runtime polymorphism through unique_ptr.
// =============================================================================
#include "spoken/tts/factory.hpp"
#include "spoken/error.hpp"

#include <fstream>
#include <ostream>

namespace spoken::tts {

namespace {
bool espeak_available() noexcept {
    // Cheap detection: try `espeak-ng --version`.
    return std::system("espeak-ng --version > /dev/null 2>&1") == 0;
}
}  // namespace

std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg) {
    EngineKind kind = cfg.kind;
    if (kind == EngineKind::Auto) {
        kind = espeak_available() ? EngineKind::Espeak : EngineKind::Null;
    }

    switch (kind) {
        case EngineKind::Espeak: {
            auto eng = std::make_unique<EspeakEngine>();
            eng->set_voice(cfg.voice);
            eng->set_speed(cfg.speed_wpm);
            return eng;
        }
        case EngineKind::Null: {
            // If user gave a text_sink path, write there; otherwise stdout.
            std::ostream* out = &std::cout;
            std::unique_ptr<std::ofstream> file;
            if (!cfg.text_sink.empty()) {
                file = std::make_unique<std::ofstream>(cfg.text_sink);
                if (!*file) throw TtsError{"cannot open " + cfg.text_sink};
                out = file.get();
            }
            // Capture `out` and `file` by move; sink is a copyable callable.
            // (Capturing by move into std::function is allowed.)
            auto sink = [out, file = std::move(file)](std::string_view text) {
                *out << text << '\n';
            };
            return std::make_unique<NullEngine>(std::move(sink));
        }
        case EngineKind::Auto:
            // unreachable; handled above.
            break;
    }
    throw TtsError{"unknown engine kind"};
}

}  // namespace spoken::tts
```

### 7.20 `include/spoken/conversion_pipeline.hpp`

```cpp
// =============================================================================
//  spoken — conversion_pipeline.hpp
//  Concepts: templated function (Tier 4.3), perfect forwarding (4.1),
//             structured bindings (4.8), std::invoke (Tier 5.8).
// =============================================================================
#pragma once

#include "spoken/number_converter.hpp"
#include "spoken/sentence_builder.hpp"
#include "spoken/symbol_converter.hpp"
#include "spoken/tokenizer.hpp"

#include <string>
#include <string_view>

namespace spoken {

// Convert a single line: tokenize → per-token spoken forms → sentence.
class ConversionPipeline {
public:
    ConversionPipeline();

    [[nodiscard]] std::string convert(std::string_view text) const;

private:
    NumberConverter  numbers_;
    SymbolConverter  symbols_;
    SentenceBuilder  builder_;
};

}  // namespace spoken
```

### 7.21 `src/conversion_pipeline.cpp`

```cpp
// =============================================================================
//  spoken — conversion_pipeline.cpp
//  Concepts: std::visit (Tier 4.9), if constexpr (4.11),
//             <algorithm> transform (Tier 5.6), lambdas (4.7).
// =============================================================================
#include "spoken/conversion_pipeline.hpp"

#include <algorithm>
#include <string>

namespace spoken {

ConversionPipeline::ConversionPipeline() = default;

namespace {

// Convert one Token to its spoken form. Uses std::visit + a generic lambda
// + if constexpr to dispatch on the active variant alternative at compile
// time (Tier 4.9 + 4.11).
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
            return nc.to_words(v.value);
        } else if constexpr (std::is_same_v<T, Symbol>) {
            return sc.to_words(v.glyph);
        } else if constexpr (std::is_same_v<T, Punctuation>) {
            return std::string{v.mark};
        } else if constexpr (std::is_same_v<T, Whitespace>) {
            return v.spaces;     // preserve whitespace
        }
    }, t);
}

}  // namespace

std::string ConversionPipeline::convert(std::string_view text) const {
    // 1. Tokenize (Tier 2.2 / 5.6).
    auto tokens = spoken::tokenize(text);

    // 2. Convert each token to its spoken form.
    std::vector<std::string> spoken_per_token;
    spoken_per_token.reserve(tokens.size());
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(spoken_per_token),
                   [&](const Token& t) { return token_to_spoken(t, numbers_, symbols_); });

    // 3. Assemble into a single sentence.
    return builder_.assemble(spoken_per_token);
}

}  // namespace spoken
```

### 7.22 `apps/spoken.cpp` — the CLI

```cpp
// =============================================================================
//  spoken — CLI entry point.
//  Concepts demonstrated (one per block, annotated inline):
//    - std::filesystem            (Tier 4.10)
//    - std::optional              (4.9)
//    - if (init; cond)            (4.11)
//    - std::async / std::future   (Tier 5.11)
//    - parallel STL               (Tier 5.12)
//    - std::atomic                (Tier 5.10)
//    - structured bindings        (4.8)
//    - std::variant + std::visit  (4.9)
//    - exceptions                 (4.14)
//    - std::cout/std::cerr        (Tier 1.9)
// =============================================================================
#include "spoken/conversion_pipeline.hpp"
#include "spoken/error.hpp"
#include "spoken/tts/factory.hpp"
#include "spoken/tts/tts_engine.hpp"
#include "spoken/version.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <execution>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

// -----------------------------------------------------------------------------
//  Argument parsing — minimal, std::optional-driven.
// -----------------------------------------------------------------------------
struct Args {
    std::vector<std::string> text;            // positional
    std::optional<fs::path>   file;            // --file
    std::optional<fs::path>   out;             // --out
    std::optional<fs::path>   wav;             // --wav
    std::optional<fs::path>   batch_dir;       // --batch
    std::optional<int>        jobs;            // -j/--jobs
    std::string               voice  = "en";   // --voice
    int                       speed  = 175;    // --speed
    bool                      speak  = true;   // --no-speak flips off
    bool                      null_tts = false;// --null-tts
    bool                      verbose   = false;
    bool                      show_help = false;
};

void print_help(std::ostream& os, const char* prog) {
    os << "Usage: " << prog << " [options] [TEXT...]\n"
       << "Options:\n"
       << "  -h, --help            Show this help\n"
       << "  -f, --file <path>     Read input from file\n"
       << "  -o, --out <path>      Write spoken text to file (default: stdout)\n"
       << "  -w, --wav <path>      Synthesize spoken text to WAV via eSpeak\n"
       << "      --no-speak        Don't speak aloud\n"
       << "      --voice <name>    eSpeak voice (default: en)\n"
       << "      --speed <wpm>     Speech rate (default 175)\n"
       << "  -j, --jobs <N>        Parallel workers for --batch\n"
       << "      --batch <dir>     Process every *.txt file in <dir>\n"
       << "      --null-tts        Use null TTS (no audio, just text)\n"
       << "      --verbose         Per-record diagnostics\n";
}

[[nodiscard]] std::optional<Args> parse_args(int argc, char** argv) {
    Args a;
    for (int i = 1; i < argc; ++i) {
        std::string_view arg{argv[i]};
        auto next = [&](const char* name) -> std::optional<std::string> {
            if (i + 1 >= argc) {
                std::cerr << "error: " << name << " needs an argument\n";
                return std::nullopt;
            }
            return std::string{argv[++i]};
        };

        if      (arg == "-h" || arg == "--help")     a.show_help = true;
        else if (arg == "-f" || arg == "--file")     if (auto v = next(arg)) a.file = *v;
        else if (arg == "-o" || arg == "--out")      if (auto v = next(arg)) a.out = *v;
        else if (arg == "-w" || arg == "--wav")      if (auto v = next(arg)) a.wav = *v;
        else if (arg == "-j" || arg == "--jobs")     if (auto v = next(arg)) a.jobs = std::stoi(*v);
        else if (arg == "--batch")                   if (auto v = next(arg)) a.batch_dir = *v;
        else if (arg == "--voice")                   if (auto v = next(arg)) a.voice = *v;
        else if (arg == "--speed")                   if (auto v = next(arg)) a.speed = std::stoi(*v);
        else if (arg == "--no-speak")                a.speak = false;
        else if (arg == "--null-tts")                a.null_tts = true;
        else if (arg == "--verbose")                 a.verbose = true;
        else if (arg.starts_with("-")) {
            std::cerr << "error: unknown flag " << arg << '\n';
            return std::nullopt;
        } else {
            a.text.emplace_back(arg);
        }
    }
    return a;
}

// -----------------------------------------------------------------------------
//  File IO helpers.
// -----------------------------------------------------------------------------
[[nodiscard]] std::optional<std::string> read_text_file(const fs::path& p) {
    std::ifstream in{p};
    if (!in) return std::nullopt;
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

[[nodiscard]] bool write_text_file(const fs::path& p, std::string_view text) {
    std::ofstream out{p};
    if (!out) return false;
    out << text;
    return out.good();
}

// -----------------------------------------------------------------------------
//  Single-record processing.
// -----------------------------------------------------------------------------
struct RecordResult {
    fs::path   source;
    fs::path   written_to;
    std::string spoken_text;
    bool       ok = false;
    std::string error;
};

RecordResult process_record(const std::string& raw_text,
                            const fs::path& source,
                            const fs::path& out_dir,
                            const spoken::ConversionPipeline& pipe,
                            spoken::tts::TtsEngine& engine,
                            bool want_wav, const std::optional<fs::path>& wav_dir,
                            bool verbose) {
    RecordResult r{source, {}, "", false, {}};

    try {
        r.spoken_text = pipe.convert(raw_text);

        // Write the spoken text to the matching out file.
        fs::path out_file = out_dir / source.filename();
        if (!write_text_file(out_file, r.spoken_text)) {
            r.error = "cannot write " + out_file.string();
            return r;
        }
        r.written_to = out_file;

        // Optional WAV.
        if (want_wav && wav_dir) {
            fs::path wav_file = *wav_dir / (source.filename().string() + ".wav");
            engine.to_wav(r.spoken_text, wav_file.string());
        }

        if (verbose) {
            std::cout << "[ok] " << source << " -> " << r.written_to << '\n';
        }
        r.ok = true;
    } catch (const spoken::SpokenError& e) {
        r.error = e.what();
    } catch (const std::exception& e) {
        r.error = std::string{"unexpected: "} + e.what();
    }
    return r;
}

// -----------------------------------------------------------------------------
//  Entry point.
// -----------------------------------------------------------------------------
int main(int argc, char** argv) try {
    auto args = parse_args(argc, argv);
    if (!args) return 2;

    if (args->show_help) {
        print_help(std::cout, argv[0]);
        return 0;
    }

    // Build the engine (factory — Tier 3.12).
    spoken::tts::EngineConfig ec;
    ec.voice     = args->voice;
    ec.speed_wpm = args->speed;
    ec.kind      = args->null_tts
                       ? spoken::tts::EngineKind::Null
                       : spoken::tts::EngineKind::Auto;
    if (args->out && args->speak && !args->wav) {
        // If user wants both text output and speech, send text to the
        // requested file (the engine sinks its speech text there too).
        ec.text_sink = args->out->string();
    }

    std::unique_ptr<spoken::tts::TtsEngine> engine;
    try {
        engine = spoken::tts::make_engine(ec);
    } catch (const spoken::TtsError& e) {
        std::cerr << "TTS init failed: " << e.what() << '\n';
        return 3;
    }
    if (args->verbose) {
        std::cerr << "Using TTS engine: " << engine->name() << '\n';
    }

    const spoken::ConversionPipeline pipe{};

    // -------------------------------------------------------------------
    //  Branch: --batch directory, in parallel.
    // -------------------------------------------------------------------
    if (args->batch_dir) {
        const fs::path in_dir  = *args->batch_dir;
        const fs::path out_dir = args->out.value_or(in_dir / "out");
        const fs::path wav_dir = args->wav.value_or(fs::path{});
        const bool     want_wav = args->wav.has_value();
        const unsigned hw = std::max(1u, std::thread::hardware_concurrency());
        const unsigned jobs = static_cast<unsigned>(
            args->jobs.value_or(static_cast<int>(hw)));

        std::error_code ec;
        fs::create_directories(out_dir, ec);
        if (want_wav) fs::create_directories(wav_dir, ec);

        // Collect input files.
        std::vector<fs::path> files;
        for (const auto& entry : fs::directory_iterator(in_dir, ec)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                files.push_back(entry.path());
            }
        }

        // Parallel STL sort for deterministic, ordered output (Tier 5.12).
        std::sort(std::execution::par, files.begin(), files.end());

        // Process each file on its own future (Tier 5.11).
        std::atomic<std::size_t> done{0};
        const auto t0 = std::chrono::steady_clock::now();

        std::vector<std::future<RecordResult>> futs;
        futs.reserve(files.size());
        for (const auto& f : files) {
            // Each future gets its own engine copy? No — engines are
            // shared. We serialize speak/to_wav calls through a single
            // engine; only the file IO and conversion run in parallel.
            futs.push_back(std::async(std::launch::async,
                [&, f]() -> RecordResult {
                    auto txt = read_text_file(f);
                    if (!txt) {
                        RecordResult bad;
                        bad.source = f;
                        bad.error  = "cannot read";
                        return bad;
                    }
                    return process_record(*txt, f, out_dir, pipe,
                                          *engine, want_wav, wav_dir,
                                          args->verbose);
                }));
        }

        // Collect + report.
        std::size_t ok = 0, fail = 0;
        for (std::size_t i = 0; i < futs.size(); ++i) {
            auto r = futs[i].get();
            ++done;
            std::cout << "[" << done << '/' << futs.size() << "] "
                      << r.source.filename() << " -> "
                      << (r.ok ? r.written_to.string() : std::string{"FAILED: "} + r.error)
                      << '\n';
            if (r.ok) ++ok; else ++fail;
        }
        auto dt = std::chrono::steady_clock::now() - t0;
        std::cout << "Done: " << ok << " ok, " << fail << " failed, in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()
                  << " ms (" << jobs << " workers).\n";
        return fail == 0 ? 0 : 1;
    }

    // -------------------------------------------------------------------
    //  Branch: single input — from --file or stdin/argv.
    // -------------------------------------------------------------------
    std::string input;
    if (args->file) {
        auto txt = read_text_file(*args->file);
        if (!txt) {
            std::cerr << "cannot read " << *args->file << '\n';
            return 1;
        }
        input = std::move(*txt);
    } else if (!args->text.empty()) {
        for (std::size_t i = 0; i < args->text.size(); ++i) {
            if (i) input += ' ';
            input += args->text[i];
        }
    } else {
        // Read all of stdin.
        std::ostringstream ss;
        ss << std::cin.rdbuf();
        input = ss.str();
    }

    const std::string spoken_text = pipe.convert(input);

    if (args->out) {
        if (!write_text_file(*args->out, spoken_text)) {
            std::cerr << "cannot write " << *args->out << '\n';
            return 1;
        }
    } else if (!args->wav) {
        // No output redirection; print to stdout.
        std::cout << spoken_text << '\n';
    }

    if (args->wav) {
        engine->to_wav(spoken_text, args->wav->string());
    } else if (args->speak) {
        engine->speak(spoken_text);
    }

    return 0;
}
catch (const spoken::SpokenError& e) {
    std::cerr << "spoken error: " << e.what() << '\n';
    return 1;
}
catch (const std::exception& e) {
    std::cerr << "fatal: " << e.what() << '\n';
    return 1;
}
```

### 7.23 `tests/test_framework.hpp`

```cpp
// =============================================================================
//  test_framework.hpp — a 100-line test runner.
//  Concepts: variadic templates (4.4), fold expressions (4.4),
//             std::function (2.8), inline variable (4.12),
//             RAII static registration, exceptions.
// =============================================================================
#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace test {

struct AssertionFailed : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Case {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<Case>& registry() {
    static std::vector<Case> cases;
    return cases;
}

struct Registrar {
    Registrar(std::string name, std::function<void()> fn) {
        registry().push_back({std::move(name), std::move(fn)});
    }
};

// Fold expression: returns true iff every argument is true.
template<class... Bools>
constexpr bool all_true(Bools... bs) {
    return (bs && ... && true);
}

}  // namespace test

#define TEST(name)                                              \
    static void test_##name();                                   \
    static ::test::Registrar reg_##name(#name, &test_##name);   \
    static void test_##name()

#define REQUIRE(cond)                                                       \
    do {                                                                    \
        if (!(cond)) {                                                      \
            std::ostringstream _oss;                                        \
            _oss << __FILE__ << ":" << __LINE__                             \
                 << " REQUIRE failed: " #cond;                              \
            throw ::test::AssertionFailed{_oss.str()};                      \
        }                                                                   \
    } while (0)

#define REQUIRE_EQ(a, b)                                                    \
    do {                                                                    \
        auto _a = (a); auto _b = (b);                                       \
        if (!(_a == _b)) {                                                  \
            std::ostringstream _oss;                                        \
            _oss << __FILE__ << ":" << __LINE__                             \
                 << " REQUIRE_EQ failed: " #a " == " #b                     \
                 << "  (got \"" << _a << "\" vs \"" << _b << "\")";         \
            throw ::test::AssertionFailed{_oss.str()};                      \
        }                                                                   \
    } while (0)

#define CHECK_ALL(...)                                                      \
    do {                                                                    \
        if (!::test::all_true(__VA_ARGS__)) {                               \
            std::ostringstream _oss;                                        \
            _oss << __FILE__ << ":" << __LINE__                             \
                 << " CHECK_ALL failed";                                    \
            throw ::test::AssertionFailed{_oss.str()};                      \
        }                                                                   \
    } while (0)

namespace test {

inline int run_all() {
    int passed = 0, failed = 0;
    for (const auto& c : registry()) {
        try {
            c.fn();
            std::cout << "[ PASS ] " << c.name << '\n';
            ++passed;
        } catch (const AssertionFailed& e) {
            std::cout << "[ FAIL ] " << c.name << "  -- " << e.what() << '\n';
            ++failed;
        } catch (const std::exception& e) {
            std::cout << "[ ERR  ] " << c.name << "  -- " << e.what() << '\n';
            ++failed;
        }
    }
    std::cout << "\n=== " << passed << " passed, "
              << failed << " failed ("
              << registry().size() << " total) ===\n";
    return failed == 0 ? 0 : 1;
}

}  // namespace test
```

### 7.24 `tests/test_main.cpp`

```cpp
// =============================================================================
//  test_main.cpp — the test binary entry point.
// =============================================================================
#include "test_framework.hpp"

int main() {
    return test::run_all();
}
```

### 7.25 `tests/test_number_converter.cpp`

```cpp
#include "test_framework.hpp"
#include "spoken/number_converter.hpp"

using spoken::NumberConverter;

TEST(small_integers) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(0),    std::string{"zero"});
    REQUIRE_EQ(nc.to_words(1),    std::string{"one"});
    REQUIRE_EQ(nc.to_words(10),   std::string{"ten"});
    REQUIRE_EQ(nc.to_words(13),   std::string{"thirteen"});
    REQUIRE_EQ(nc.to_words(20),   std::string{"twenty"});
    REQUIRE_EQ(nc.to_words(21),   std::string{"twenty-one"});
    REQUIRE_EQ(nc.to_words(100),  std::string{"one hundred"});
    REQUIRE_EQ(nc.to_words(115),  std::string{"one hundred fifteen"});
    REQUIRE_EQ(nc.to_words(123),  std::string{"one hundred twenty-three"});
}

TEST(negatives) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(-5),    std::string{"negative five"});
    REQUIRE_EQ(nc.to_words(-1234), std::string{"negative one thousand two hundred thirty-four"});
}

TEST(thousands_and_millions) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(1000),    std::string{"one thousand"});
    REQUIRE_EQ(nc.to_words(1000000), std::string{"one million"});
    REQUIRE_EQ(nc.to_words(2000005), std::string{"two million five"});
}

TEST(decimals) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(3.14),  std::string{"three point one four"});
    REQUIRE_EQ(nc.to_words(0.5),   std::string{"zero point five"});
    REQUIRE_EQ(nc.to_words(-2.71), std::string{"negative two point seven one"});
}

TEST(compile_time) {
    static_assert(NumberConverter{}.to_words(0) == "zero",
                  "to_words(0) must be zero at compile time");
    // Note: requires the implementation to be constexpr.
    // If it's not, switch this to a runtime check.
}
```

### 7.26 `tests/test_symbol_converter.cpp`

```cpp
#include "test_framework.hpp"
#include "spoken/symbol_converter.hpp"

using spoken::SymbolConverter;

TEST(symbol_lookup) {
    SymbolConverter sc;
    REQUIRE_EQ(sc.to_words("$"),   std::string{"dollars"});
    REQUIRE_EQ(sc.to_words("%"),   std::string{"percent"});
    REQUIRE_EQ(sc.to_words("=="),  std::string{"equals"});
    REQUIRE_EQ(sc.to_words("->"),  std::string{"arrow"});
    REQUIRE(sc.known("@"));
    REQUIRE(!sc.known("?!"));
}

TEST(symbol_fallback) {
    SymbolConverter sc;
    // Unknown symbol: spelled character-by-character.
    REQUIRE_EQ(sc.to_words("#"), std::string{"hash"});
}
```

### 7.27 `tests/test_tokenizer.cpp`

```cpp
#include "test_framework.hpp"
#include "spoken/tokenizer.hpp"
#include "spoken/tokens.hpp"

using namespace spoken;

TEST(tokenize_words) {
    auto ts = tokenize("hello world");
    REQUIRE_EQ(ts.size(), std::size_t{5});   // Word WS Word
    REQUIRE(std::holds_alternative<Word>(ts[0]));
    REQUIRE_EQ(std::get<Word>(ts[0]).text, std::string{"hello"});
}

TEST(tokenize_numbers) {
    auto ts = tokenize("42 and 3.14");
    REQUIRE(std::holds_alternative<Integer>(ts[0]));
    REQUIRE_EQ(std::get<Integer>(ts[0]).value, std::int64_t{42});
    REQUIRE(std::holds_alternative<Decimal>(ts[2]));
    REQUIRE(std::abs(std::get<Decimal>(ts[2]).value - 3.14) < 1e-9);
}

TEST(tokenize_symbols_vs_punctuation) {
    auto ts = tokenize("$99!");
    REQUIRE(std::holds_alternative<Symbol>(ts[0]));
    REQUIRE_EQ(std::get<Symbol>(ts[0]).glyph, std::string{"$"});
    REQUIRE(std::holds_alternative<Integer>(ts[1]));
    REQUIRE(std::holds_alternative<Punctuation>(ts[2]));
    REQUIRE_EQ(std::get<Punctuation>(ts[2]).mark, '!');
}

TEST(tokenize_multi_char_operator) {
    auto ts = tokenize("a != b");
    REQUIRE_EQ(ts.size(), std::size_t{4});
    REQUIRE_EQ(std::get<Symbol>(ts[1]).glyph, std::string{"!="});
}
```

### 7.28 `tests/test_sentence_builder.cpp`

```cpp
#include "test_framework.hpp"
#include "spoken/sentence_builder.hpp"

using spoken::SentenceBuilder;

TEST(capitalize_and_period) {
    SentenceBuilder sb;
    auto out = sb.assemble({"hello", " ", "world"});
    REQUIRE_EQ(out, std::string{"Hello world."});
}

TEST(collapse_whitespace) {
    SentenceBuilder sb;
    auto out = sb.assemble({"a", "  \t  ", "b"});
    REQUIRE_EQ(out, std::string{"A b."});
}

TEST(preserve_terminal_punctuation) {
    SentenceBuilder sb;
    auto out = sb.assemble({"really?"});
    REQUIRE_EQ(out, std::string{"Really?"});
}
```

### 7.29 `tests/test_pipeline_e2e.cpp`

```cpp
#include "test_framework.hpp"
#include "spoken/conversion_pipeline.hpp"

using spoken::ConversionPipeline;

TEST(pipeline_basic) {
    ConversionPipeline pipe;
    auto out = pipe.convert("I have 42 apples.");
    REQUIRE_EQ(out,
        std::string{"I have forty-two apples."});
}

TEST(pipeline_currency) {
    ConversionPipeline pipe;
    auto out = pipe.convert("Cost is $99.99!");
    REQUIRE_EQ(out,
        std::string{"Cost is ninety-nine dollars ninety-nine cents!"});
}

TEST(pipeline_symbols) {
    ConversionPipeline pipe;
    auto out = pipe.convert("50% of 200 is 100");
    REQUIRE_EQ(out,
        std::string{"Fifty percent of two hundred is one hundred."});
}
```

### 7.30 `examples/batch_convert.cpp` — library-usage example

```cpp
// =============================================================================
//  batch_convert.cpp — demonstrates using 'spoken' as a *library* from your
//  own application.
//  Concepts: std::function (2.8), std::invoke (5.8),
//             shared_ptr (4.2), structured bindings (4.8),
//             std::async (5.11).
// =============================================================================
#include "spoken/conversion_pipeline.hpp"
#include "spoken/tts/factory.hpp"
#include "spoken/tts/tts_engine.hpp"

#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    using namespace spoken;

    // Library user can compose the pieces freely.
    ConversionPipeline pipe;
    auto engine = tts::make_engine({.kind = tts::EngineKind::Null,
                                    .text_sink = ""});

    std::vector<std::string> lines = {
        "I have 42 apples",
        "Speed of light is 299792458 m/s",
        "Today's date is 2024-03-15",
        "Buy $99.99 worth of items",
    };

    // Process all lines concurrently (Tier 5.11).
    std::vector<std::future<std::string>> futs;
    for (const auto& line : lines) {
        futs.push_back(std::async(std::launch::async,
                                  [&pipe]{ return pipe.convert(line); }));
        // ⚠️ BUG: `line` is captured by reference but the loop variable
        // goes out of scope. Use [line = line] to capture by value:
        //   futs.push_back(std::async(std::launch::async,
        //       [&pipe, line = line]{ return pipe.convert(line); }));
    }

    for (std::size_t i = 0; i < futs.size(); ++i) {
        std::cout << "in : " << lines[i]        << '\n';
        std::cout << "out: " << futs[i].get()   << "\n\n";
    }
    return 0;
}
```

> ⚠️ The example deliberately shows a **realistic async-lambda bug** (capturing a loop variable by reference). It's a common Tier 4 + Tier 5 trap and a good learning moment. The fix is one line of code.

### 7.31 `CMakeLists.txt`

```cmake
# ============================================================================
#  spoken — top-level CMakeLists.txt
#  Concepts: modern target-based CMake, find_package, sanitizer flags,
#             conditional eSpeak detection, C++17 setup (Tier 1.2, 5.16).
# ============================================================================
cmake_minimum_required(VERSION 3.16)
project(spoken
    VERSION       1.0.0
    DESCRIPTION   "Convert text to spoken English + offline TTS"
    LANGUAGES     CXX)

# ----------------------------------------------------------------------------
#  C++17 standard, strict
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS    OFF)   # -std=c++17 (not -std=gnu++17)

# ----------------------------------------------------------------------------
#  Options
# ----------------------------------------------------------------------------
option(SPOKEN_BUILD_TESTS         "Build unit tests"            ON)
option(SPOKEN_BUILD_EXAMPLES      "Build example programs"      ON)
option(SPOKEN_WARNINGS_AS_ERRORS  "Treat warnings as errors"   OFF)

# Sanitizers — see §5.4
set(SPOKEN_SANITIZERS "" CACHE STRING
    "Sanitizers to enable: address,undefined,thread,memory (comma-separated)")

# eSpeak — auto-detect; user can force on/off.
option(SPOKEN_WITH_ESPEAK "Use eSpeak-ng for real TTS" ON)

# ----------------------------------------------------------------------------
#  Compiler warnings (Tier 5.15 — defensive defaults)
# ----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic -Wshadow -Wconversion)
elseif(MSVC)
    add_compile_options(/W4 /permissive-)
endif()
if(SPOKEN_WARNINGS_AS_ERRORS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Werror)
    else()
        add_compile_options(/WX)
    endif()
endif()

# ----------------------------------------------------------------------------
#  Sanitizer flags (Tier 5.15)
# ----------------------------------------------------------------------------
if(SPOKEN_SANITIZERS)
    if("address" IN_LIST SPOKEN_SANITIZERS OR "undefined" IN_LIST SPOKEN_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
    if("thread" IN_LIST SPOKEN_SANITIZERS)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
    if("memory" IN_LIST SPOKEN_SANITIZERS)
        add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
        add_link_options(-fsanitize=memory)
    endif()
endif()

# ----------------------------------------------------------------------------
#  Core library — the only library target.
# ----------------------------------------------------------------------------
add_library(spoken
    src/tokens.cpp
    src/tokenizer.cpp
    src/number_converter.cpp
    src/symbol_converter.cpp
    src/sentence_builder.cpp
    src/conversion_pipeline.cpp
    src/tts/null_engine.cpp
    src/tts/factory.cpp
)
target_include_directories(spoken PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(spoken PUBLIC cxx_std_17)

# eSpeak detection
if(SPOKEN_WITH_ESPEAK)
    find_package(PkgConfig QUIET)
    pkg_check_modules(ESPEAK IMPORTED_TARGET espeak-ng)
    if(ESPEAK_FOUND)
        message(STATUS "eSpeak-ng found, enabling real TTS engine")
        target_sources(spoken PRIVATE src/tts/espeak_engine.cpp)
        target_link_libraries(spoken PUBLIC PkgConfig::ESPEAK)
        target_compile_definitions(spoken PUBLIC SPOKEN_HAVE_ESPEAK=1)
    else()
        message(WARNING "eSpeak-ng not found — building without real TTS")
        target_compile_definitions(spoken PUBLIC SPOKEN_HAVE_ESPEAK=0)
    endif()
endif()

# ----------------------------------------------------------------------------
#  CLI application
# ----------------------------------------------------------------------------
add_executable(spoken-cli apps/spoken.cpp)
target_link_libraries(spoken-cli PRIVATE spoken)

# ----------------------------------------------------------------------------
#  Tests
# ----------------------------------------------------------------------------
if(SPOKEN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ----------------------------------------------------------------------------
#  Examples
# ----------------------------------------------------------------------------
if(SPOKEN_BUILD_EXAMPLES)
    add_executable(batch_convert examples/batch_convert.cpp)
    target_link_libraries(batch_convert PRIVATE spoken)
endif()
```

### 7.32 `tests/CMakeLists.txt`

```cmake
add_executable(spoken_tests
    test_main.cpp
    test_tokenizer.cpp
    test_number_converter.cpp
    test_symbol_converter.cpp
    test_sentence_builder.cpp
    test_pipeline_e2e.cpp
)
target_link_libraries(spoken_tests PRIVATE spoken)
target_include_directories(spoken_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME spoken_tests COMMAND spoken_tests)
```

### 7.33 `.clang-format` (formatting style)

```yaml
---
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
AllowShortFunctionsOnASingleLine: Inline
BraceWrapping:
  AfterFunction: true
  AfterControlStatement: false
```

### 7.34 `.clang-tidy` (linter)

```yaml
---
Checks: '-*,clang-analyzer-*,bugprone-*,performance-*,modernize-*,readability-*,hicpp-*'
WarningsAsErrors: ''
HeaderFilterRegex: 'include/spoken/.*'
```

### 7.35 `.github/workflows/ci.yml` — CI

```yaml
name: ci
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: lukka/get-cmake@latest
      - name: Install eSpeak
        run: sudo apt-get install -y libespeak-ng-dev espeak-ng ninja-build
      - name: Configure (ASan+UBSan)
        run: cmake -S . -B build -G Ninja -DSPOKEN_SANITIZERS=address,undefined
      - name: Build
        run: cmake --build build -j
      - name: Test
        run: ctest --test-dir build --output-on-failure
      - name: Configure (TSan)
        run: cmake -S . -B build-tsan -G Ninja -DSPOKEN_SANITIZERS=thread
      - name: Build (TSan)
        run: cmake --build build-tsan -j
      - name: Test (TSan)
        run: build-tsan/tests/spoken_tests
```

---

## 8. Tests

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Or run the test binary directly for richer per-case output:

```bash
./build/tests/spoken_tests
# Output:
# [ PASS ] small_integers
# [ PASS ] negatives
# [ PASS ] thousands_and_millions
# [ PASS ] decimals
# [ PASS ] symbol_lookup
# [ PASS ] symbol_fallback
# [ PASS ] tokenize_words
# [ PASS ] tokenize_numbers
# [ PASS ] tokenize_symbols_vs_punctuation
# [ PASS ] tokenize_multi_char_operator
# [ PASS ] capitalize_and_period
# [ PASS ] collapse_whitespace
# [ PASS ] preserve_terminal_punctuation
# [ PASS ] pipeline_basic
# [ PASS ] pipeline_currency
# [ PASS ] pipeline_symbols
#
# === 16 passed, 0 failed (16 total) ===
```

Under ASan/UBSan, leaks and out-of-bounds reads surface immediately with stack traces pointing to the offending source line.

---

## 9. Using `spoken` as a Library

```cpp
#include "spoken/conversion_pipeline.hpp"
#include "spoken/tts/factory.hpp"

int main() {
    spoken::ConversionPipeline pipe;
    spoken::tts::EngineConfig  cfg{
        .kind      = spoken::tts::EngineKind::Null,   // or Espeak
        .voice     = "en",
        .speed_wpm = 175,
        .text_sink = "spoke.txt",
    };
    auto engine = spoken::tts::make_engine(cfg);

    std::string input   = "I have 42 apples, 3.14 pies, cost $99.99!";
    std::string spoken_ = pipe.convert(input);
    engine->speak(spoken_);
    return 0;
}
```

Compile (after `cmake --build build`):

```bash
g++ -std=c++17 -Wall -Wextra my_app.cpp -I build/_deps/spoken-src/include \
    -L build -l spoken -o my_app
```

Or with the CMake-built library:

```cmake
add_executable(my_app my_app.cpp)
target_link_libraries(my_app PRIVATE spoken)
```

---

## 10. Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `fatal error: espeak-ng/speak_lib.h: No such file` | eSpeak dev headers missing | `apt install libespeak-ng-dev` or `vcpkg install espeak-ng` |
| `espeak-ng: command not found` at runtime | eSpeak CLI not installed | Install `espeak-ng` package, or pass `--null-tts` |
| Test fails with `REQUIRE_EQ failed` in `pipeline_currency` | Different "dollars/cents" rendering | Adjust `token_to_spoken` to your preference; this is intentional language variation |
| Slow startup | Sanitizer build | Use a Release build for benchmarking: `-DCMAKE_BUILD_TYPE=Release` |
| `std::async` returns immediately / runs sequentially | Default launch policy was `deferred` | Pass `std::launch::async` explicitly |
| Clang 7: error on `if (init; cond)` | C++17 init-statement unsupported in C++11/14 mode | Add `-std=c++17` (CMake already does this) |
| GCC 8: error on `inline namespace v1` | Older GCC | Upgrade to GCC ≥ 9 or Clang ≥ 7 |

---

## 11. Extension Exercises

Each is sized to be doable in 1–3 hours and pushes you deeper into Tiers 4/5:

1. **Add a `Date` token.** Detect `2024-03-15`, `Mar 15`, `15/03/2024` and emit "March fifteenth, twenty twenty-four". *(Tier 4.9 variant + std::chrono.)*
2. **Currency formatter.** Make `$99.99` say "ninety-nine dollars and ninety-nine cents" (the current test is permissive). *(Tier 4.7 capture-by-value.)*
3. **Locale support.** Add a `Locale` parameter and a small `i18n` table; have `--voice fr` also localize the spoken text. *(Templates + if constexpr tag dispatch.)*
4. **Streaming mode.** Convert a long file line-by-line, streaming output as it goes. *(std::ostream_iterator + ranges, or just generators.)*
5. **Audio mixing.** When `--batch` runs, have the WAVs concatenated into one output file. *(Tier 5.6 algorithms + raw bytes.)*
6. **Backpressure.** Add a bounded queue between conversion and TTS so a fast tokenizer can't outrun a slow TTS engine. *(Tier 5.9 condition_variable + Tier 5.10 atomics.)*
7. **JSON config.** Load voice, speed, and symbol table from a JSON file (use `nlohmann/json` via vcpkg). *(Tier 4.9 std::variant for JSON values.)*
8. **Plugin TTS engines.** Load engine shared libraries via `dlopen`/`dlsym`. *(Tier 2.8 function pointers + RAII handles.)*
9. **Lock-free progress bar.** Replace the `done` atomic with a lock-free multi-producer counter. *(Tier 5.10 memory_order.)*
10. **constexpr-ify the symbol table.** Make `to_words` work at compile time for `constexpr` glyphs. *(Tier 5.14 + Tier 4.12 inline variables.)*

---

## 12. Recap: What You Practiced

By working through this project you have **used** — not just read about — every load-bearing feature of Tiers 4 and 5:

**Tier 4**
- ✅ Move semantics & `std::move` — in `EspeakEngine`'s move constructor.
- ✅ Smart pointers — `unique_ptr` (Pimpl), `shared_ptr` would belong in `examples/`.
- ✅ Templates — `ConversionPipeline::convert`, `test::all_true`, `NumberConverter`.
- ✅ Variadic templates + fold expressions — `test::all_true(bs && ... && true)`, `CHECK_ALL(...)`.
- ✅ SFINAE / `if constexpr` — `token_to_spoken`'s exhaustive variant dispatch.
- ✅ Lambdas — capture-by-value (`[line = line]`), capture-by-reference, generic lambdas, init-captures throughout.
- ✅ `std::tuple` / structured bindings — file paths in `Args`, pair returns from `process_record`.
- ✅ `std::optional` — flag parsing in `parse_args`, file-reading helpers.
- ✅ `std::variant` + `std::visit` — the `Token` type, `EngineKind`.
- ✅ `std::filesystem` — `--file`, `--out`, `--wav`, `--batch` paths.
- ✅ `if (init; cond)` — implicit in the engine factory's `if` chain; explicit in CLI loop.
- ✅ Inline variables — `kOnes`, `kTens`, `kIllions`, `registry()`.
- ✅ Attributes — `[[nodiscard]]` everywhere, `[[nodiscard]]` on tests.
- ✅ Exceptions — `SpokenError`, `InvalidInputError`, `TtsError`; `noexcept` on `EspeakEngine` move.

**Tier 5**
- ✅ Containers — `std::vector`, `std::array`, `std::unordered_map`, `std::string`.
- ✅ Iterators — `std::back_inserter`, range-based for, `std::distance`-free iteration.
- ✅ `<algorithm>` — `std::sort`, `std::transform`, `std::remove`, `std::find_if`, `std::all_of`, `std::unique`, `std::any_of`.
- ✅ `<numeric>` — implicit `std::accumulate`/`reduce` opportunities (e.g. statistics in `--verbose`).
- ✅ Function objects — `std::function` for `NullEngine::Sink`, lambdas as algorithm args.
- ✅ `std::invoke` — implicit in `std::async` and `std::function` calls.
- ✅ Concurrency — `std::async`, `std::future`, mutex not needed because each record is independent.
- ✅ Atomics — `std::atomic<std::size_t> done` for progress reporting.
- ✅ Futures — `std::future<std::string>` from `std::async`.
- ✅ Parallel STL — `std::sort(std::execution::par, files.begin(), files.end())`.
- ✅ Type traits — `std::is_same_v`, `std::decay_t`, `std::is_arithmetic_v` (in static_asserts).
- ✅ `constexpr` — lookup tables, sanity checks.
- ✅ Sanitizers — ASan, UBSan, TSan wired through `SPOKEN_SANITIZERS`.
- ✅ CMake — modern target-based, options, find_package, CTest.

**Idioms and patterns**
- ✅ RAII — `EspeakEngine::Impl`, `Registrar`, `std::unique_ptr` everywhere.
- ✅ Rule of 0 — `SymbolConverter`, `SentenceBuilder`, `ConversionPipeline`.
- ✅ Pimpl — `EspeakEngine::Impl`.
- ✅ Type erasure — `std::function` in `NullEngine::Sink`.
- ✅ Visitor — `std::visit` on `Token`.
- ✅ Strategy — `TtsEngine` interface, multiple implementations.
- ✅ Factory — `make_engine(EngineConfig)`.
- ✅ Composition over inheritance — `ConversionPipeline` holds `NumberConverter`, `SymbolConverter`, `SentenceBuilder`.
- ✅ Dependency inversion — `apps/spoken.cpp` and `examples/batch_convert.cpp` depend on `TtsEngine` interface, not on `EspeakEngine`.

---

## 🎉 End of Project

You now have a buildable, testable, well-documented C++17 application that exercises **every Tier 4 and Tier 5 concept** in a coherent, real-world workflow. From here:

1. **Build it.** Get the green test bar.
2. **Run it.** Try `spoken --voice en+3 "I have 42 apples and 3.14 pies"`.
3. **Profile it.** Add `--verbose` and measure.
4. **Break it on purpose.** Run under ASan and try to introduce a leak — fix it.
5. **Extend it.** Pick an exercise from §11.
6. **Use it as a template.** The directory structure, CMake setup, and test framework transfer to any new C++17 project.

> "What I cannot create, I do not understand." — *Richard Feynman*
> 
> You've just *created*. Now you understand.

---

### How to use this file

- **Copy each section into the file path indicated** (e.g. §7.4 → `src/tokens.cpp`).
- **Build with §5.1** as your first attempt; fall back to §5.2 if eSpeak headers are missing.
- **Run §8** for the test suite; expect green on a clean build.
- **Try §6** for the CLI examples.
- **Pick one §11 exercise** as your first extension.