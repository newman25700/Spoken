# spoken — README

> A C++17 command-line tool and reusable library that turns a line of mixed text
> (words, numbers, symbols, punctuation) into a natural spoken-English sentence, and
> optionally renders it to audio via an offline open-source TTS engine.
>
> Authoring compliance: `_mydocs/002_rules.md` (lines <= 120 chars, no tabs, no
> GitHub-style pipe-bordered tables).

---

## What it does

Given input like:

```
I have 42 apples and 3.14 pies, cost $99.99!
```

`spoken` produces:

```
I have forty-two apples and three point one four pies, cost dollars ninety-nine point nine nine!
```

It tokenizes the text, converts numbers/symbols to words, assembles a properly spaced
and capitalized sentence, and (with a real TTS engine) can speak it aloud or write a
WAV file. Without a TTS library installed it still runs fully via a "null" engine that
emits the spoken text.

---

## Status

- Builds clean under `clang++ -std=c++17 -Wall -Wextra -pedantic`.
- 18/18 unit tests pass, including under AddressSanitizer + UndefinedBehaviorSanitizer.
- CLI verified end to end (single input, stdin, file output, batch, exit codes).
- This is a teaching capstone: it deliberately exercises Tier 4 and Tier 5 C++17
  features end to end (see the reference in `000_cpp17-reference.md`).

---

## Features at a glance

- Tokenizer producing a `std::variant`-based token stream
  (word / integer / decimal / symbol / punctuation / whitespace).
- Number-to-words conversion (integers incl. negatives; decimals spoken digit by digit
  from the original literal).
- Symbol table with a spell-out fallback for unknown glyphs.
- Sentence assembly with correct spacing, capitalization, and terminal punctuation.
- Pluggable TTS via an abstract `TtsEngine` interface (null engine included; eSpeak
  engine compiled in when available).
- Parallel batch mode over a directory of `*.txt` files.

---

## Project layout

```
spoken/
  include/spoken/        public headers (library API)
    version.hpp error.hpp tokens.hpp tokenizer.hpp
    number_converter.hpp symbol_converter.hpp sentence_builder.hpp
    conversion_pipeline.hpp
    tts/tts_engine.hpp tts/null_engine.hpp tts/factory.hpp
  src/                   library sources (mirror the headers) + src/tts/
  apps/spoken.cpp        the CLI entry point
  tests/                 a tiny header-only test framework + unit tests
  CMakeLists.txt         build script (target-based, CMake >= 3.16)
```

Every source line is documented two ways: inline comments in the files themselves, and
a line-by-line companion in `009_spoken_source_documentation.md`.

---

## Build

### Option A — CMake (preferred where a generator exists)

```
cmake -S spoken -B spoken/build -DCMAKE_BUILD_TYPE=Debug -DSPOKEN_BUILD_TESTS=ON
cmake --build spoken/build -j
ctest --test-dir spoken/build --output-on-failure
```

eSpeak is auto-detected via pkg-config; if missing, the build defines
`SPOKEN_HAVE_ESPEAK=0` and uses the null engine. Install eSpeak for real audio:

- Debian/Ubuntu: `sudo apt install libespeak-ng-dev espeak-ng`
- macOS: `brew install espeak-ng`
- Windows: `vcpkg install espeak-ng`

### Option B — direct clang++ (no generator needed)

Useful on a machine that has clang but no Ninja/Make/MSVC build tool. On Windows,
first load the MSVC environment so clang can find the standard headers:

```
# Windows only: import the MSVC dev environment (PowerShell), then build.
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul && set' |
  ForEach-Object { if ($_ -match '^(INCLUDE|LIB|PATH)=(.*)$') { Set-Item ("Env:"+$matches[1]) $matches[2] } }

# Build the test binary (null-TTS path):
clang++ -std=c++17 -Wall -Wextra -pedantic -DSPOKEN_HAVE_ESPEAK=0 `
  -Ispoken/include -Ispoken/tests `
  spoken/src/tokens.cpp spoken/src/tokenizer.cpp spoken/src/number_converter.cpp `
  spoken/src/symbol_converter.cpp spoken/src/sentence_builder.cpp `
  spoken/src/conversion_pipeline.cpp spoken/src/tts/null_engine.cpp `
  spoken/src/tts/factory.cpp spoken/tests/*.cpp -o spoken/build/spoken_tests.exe

# Build the CLI (swap the test files for apps/spoken.cpp):
clang++ -std=c++17 -Wall -Wextra -pedantic -DSPOKEN_HAVE_ESPEAK=0 -Ispoken/include `
  spoken/src/*.cpp spoken/src/tts/*.cpp spoken/apps/spoken.cpp -o spoken/build/spoken-cli.exe
```

### Sanitizers

```
clang++ -std=c++17 -g -fsanitize=address,undefined -fno-omit-frame-pointer ... -o tests_asan.exe
```

On Windows the ASan runtime DLL must be on PATH, e.g.
`C:\Program Files\LLVM\lib\clang\<ver>\lib\windows`. ThreadSanitizer is not supported
on the Windows-MSVC target.

---

## Usage

```
spoken [options] [TEXT...]
spoken --file <path> [options]
echo "..." | spoken [options]
spoken --batch <dir> [options]
```

Options:

- `-h, --help`        show help
- `-f, --file <path>` read input from a file
- `-o, --out <path>`  write spoken text to a file (default stdout)
- `-w, --wav <path>`  synthesize spoken text to a WAV file
- `--no-speak`        do not speak aloud
- `--voice <name>`    voice (default `en`)
- `--speed <wpm>`     speech rate (default 175)
- `-j, --jobs <N>`    parallel workers for `--batch`
- `--batch <dir>`     process every `*.txt` in a directory
- `--null-tts`        force the null engine (text only)
- `--verbose`         per-record diagnostics

Examples:

```
spoken "Profit was -1234 dollars, up 50% from 2000005"
echo "pi is 3.14" | spoken
spoken -o out.txt "say 7 things"
spoken --batch texts --out out
```

Exit codes: `0` success, `1` runtime/IO error, `2` bad arguments,
`3` TTS init failure, `4` unexpected internal error.

---

## Library API

```cpp
#include "spoken/conversion_pipeline.hpp"
#include "spoken/tts/factory.hpp"

spoken::ConversionPipeline pipe;                 // stateless, reusable, thread-safe (const)
std::string out = pipe.convert("I have 42 apples");

spoken::tts::EngineConfig cfg;                    // built field-by-field (no designated inits)
cfg.kind = spoken::tts::EngineKind::Null;
auto engine = spoken::tts::make_engine(cfg);
engine->speak(out);
```

---

## Tests

The suite uses a small header-only framework (`tests/test_framework.hpp`) with
`TEST`, `REQUIRE`, and `REQUIRE_EQ` macros. Run via CTest or the binary directly:

```
ctest --test-dir spoken/build --output-on-failure
# or:
spoken/build/spoken_tests.exe
```

Coverage spans the tokenizer, number converter, symbol converter, sentence builder,
and end-to-end pipeline (18 cases).

---

## Design notes & decisions

- Composition over inheritance: `ConversionPipeline` owns the four converters (Rule of
  0). TTS uses an abstract interface (Strategy / Dependency Inversion).
- Whitespace tokens are produced by the tokenizer but dropped by the pipeline; the
  sentence builder owns all spacing.
- Decimals are spoken from the original lexeme (no float-formatting artifacts).
- Currency fusion ("$99.99" -> "... dollars and ... cents") is out of v1 scope; the
  grammar renders the symbol and number separately.
- Batch mode runs conversion and file IO in parallel but serializes TTS engine calls
  with a mutex (the engine is not thread-safe).

The full specification, defect register, and decisions are in
`007_spoken_master_design_and_spec.md`; the applied fixes are in
`008_spoken_corrected_impl.md`.

---

## Documentation map

- `000_cpp17-reference.md` — the C++17 tiered reference this project demonstrates.
- `001_the_project_sketch.md` — the original implementation walkthrough (sketch).
- `002` / `003` — the design spec (original and rules-compliant rendition).
- `005` / `006` — external rating and re-rating of the spec.
- `007_spoken_master_design_and_spec.md` — the CANONICAL spec (decisions + defects).
- `008_spoken_corrected_impl.md` — the corrected implementation reference.
- `009_spoken_source_documentation.md` — line-by-line source documentation.
- `010_spoken_readme.md` — this file.

---

## License

Add a license of your choice (e.g. MIT) before distributing.