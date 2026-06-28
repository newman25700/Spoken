# `spoken` — Design & Specification (Rules-Compliant Rendition)

> Status: DRAFT FOR REVIEW.
>
> This document is the rules-compliant rendition of `002_spoken_design_and_spec.md`.
> It carries the same substance, reformatted to `_mydocs/002_rules.md`
> (lines <= 120 chars, no tabs, space-aligned tables with `|---|` borders).
> The original `002_spoken_design_and_spec.md` is kept unchanged.
>
> Source artifact reviewed: `001_the_project_sketch.md` (the implementation sketch).
>
> Authority: where the sketch and this spec disagree, this spec wins and the sketch
> must be corrected. Section 11 (Defect Register) lists every known divergence.

---

## Table of Contents

1. Purpose & Scope
2. Requirements
3. Architecture Overview
4. Module Specifications & Interface Contracts
5. Data Model
6. Behavioural Specification (Normalization Rules)
7. CLI Specification
8. Concurrency Model
9. Build, Configuration & Portability
10. Test Plan & Acceptance Criteria
11. Review Findings — Defect Register
12. C++17 Conformance Checklist
13. Open Questions / Decisions Needed
14. Sign-off Checklist

---

## 1. Purpose & Scope

### 1.1 Product summary

`spoken` is a C++17 command-line tool and reusable library that converts a line of
mixed text (words, numbers, symbols, punctuation) into a natural spoken-English
sentence, and optionally renders that sentence to audio (playback or WAV) via an
offline, open-source text-to-speech (TTS) engine.

### 1.2 Goals

- G1 — Pedagogical: exercise every load-bearing Tier 4 and Tier 5 concept from
  `000_cpp17-reference.md` in one coherent, real application.
- G2 — Correct: produce deterministic, testable text output for a defined input grammar.
- G3 — Portable: build and pass all tests on Linux, macOS, and Windows with
  GCC >= 9, Clang >= 7, or MSVC >= 19.15, under `-std=c++17` strictly
  (no GNU or C++20 extensions).
- G4 — Degradable: build and run with full test coverage without any TTS library
  installed (null engine).

### 1.3 Non-goals

- Not a general NLP/grammar engine; the input grammar is the closed set in Section 6.
- Not a real-time/streaming audio mixer (those are extension exercises).
- Not internationalized in v1 (English only; locale is an extension).

### 1.4 Definitions

- Token: one lexical unit produced by the tokenizer (see Section 5.1).
- Record: one input line/file processed into one spoken sentence.
- Engine: a `TtsEngine` implementation (real `espeak` or `null`).
- Null engine: a `TtsEngine` that emits spoken text to a sink instead of audio.

---

## 2. Requirements

### 2.1 Functional requirements

- FR-1 (MUST): tokenize input into Word / Integer / Decimal / Symbol /
  Punctuation / Whitespace tokens.
- FR-2 (MUST): convert integers (including negatives, up to about +/- 9.2e18)
  to English words.
- FR-3 (MUST): convert decimals to English ("3.14" -> "three point one four").
- FR-4 (MUST): convert known symbols via a lookup table; spell unknown symbols
  by name.
- FR-5 (MUST): assemble per-token spoken forms into one capitalized, terminally
  punctuated sentence with correct spacing (no space before `.,!?;:`).
- FR-6 (MUST): read input from positional args, `--file`, or stdin.
- FR-7 (MUST): write spoken text to stdout or `--out <path>`.
- FR-8 (SHOULD): speak aloud via TTS engine unless `--no-speak`/`--null-tts`.
- FR-9 (SHOULD): synthesize WAV via `--wav <path>`.
- FR-10 (SHOULD): batch-process every `*.txt` in `--batch <dir>` concurrently,
  with deterministic ordering of the report.
- FR-11 (MUST): select engine via factory (Auto/Espeak/Null); fall back to null
  when eSpeak is unavailable.
- FR-12 (MUST): expose the conversion pipeline and factory as a linkable library API.
- FR-13 (MUST): report errors via a typed exception hierarchy; the CLI maps them
  to exit codes.

### 2.2 Non-functional requirements

- NFR-1: C++17 only. No C++20 features (`starts_with`, designated initializers,
  `consteval`, ranges) and no compiler extensions.
- NFR-2: builds clean under `-Wall -Wextra -pedantic` (and `/W4 /permissive-` on MSVC).
- NFR-3: all unit tests pass under ASan+UBSan; concurrency tests pass under TSan.
- NFR-4: the library has zero global mutable state; converters are value types
  (Rule of 0).
- NFR-5: pure conversion functions are deterministic and locale-independent
  ("C" semantics).
- NFR-6: public headers must not leak third-party C headers (eSpeak hidden behind Pimpl).
- NFR-7: CLI exit codes per Section 7.3.

---

## 3. Architecture Overview

### 3.1 Layered component view

```
Presentation:  apps/spoken-cli   examples/batch_convert
                    | depends on (interfaces only)
                    v
Domain:        ConversionPipeline           tts::TtsEngine (interface)
                 - Tokenizer                  - EspeakEngine (Pimpl)
                 - NumberConverter            - NullEngine (sink)
                 - SymbolConverter           tts::make_engine() (factory)
                 - SentenceBuilder
                    |
                    v
Foundation:    tokens (variant)   error (exception hierarchy)   version
```

Dependency rule: arrows point downward only. Presentation depends on Domain via
interfaces (`TtsEngine`) — Dependency Inversion. No layer reaches up.

### 3.2 Data flow (single record)

```
raw text -> Tokenizer -> vector<Token>
                              |  std::transform + std::visit
                              v
                    vector<string> (spoken-per-token)
                              |  SentenceBuilder::assemble
                              v
                       spoken sentence -> stdout / --out file
                                       -> TtsEngine::speak | to_wav
```

---

## 4. Module Specifications & Interface Contracts

Each module lists Responsibility, Public API (signatures), Invariants/Contracts,
and Errors. Implementation lives in the sketch; this section is the contract the
implementation must satisfy.

### 4.1 `spoken/version.hpp`

- Responsibility: compile-time version constants.
- API: `inline constexpr std::uint32_t version_{major,minor,patch};`
  and `inline constexpr const char* version_string;`.
- Contract: header-only, `inline constexpr` (ODR-safe across TUs).
  `inline namespace v1` is permitted (a C++11 feature).

### 4.2 `spoken/error.hpp`

- Responsibility: typed error hierarchy.
- API: `SpokenError : std::runtime_error` (root); `InvalidInputError : SpokenError`;
  `TtsError : SpokenError`; inheriting constructors via `using`.
- Contract: all library-thrown exceptions derive from `SpokenError`;
  `what()` is human-readable and non-empty.

### 4.3 `spoken/tokens.hpp` and `tokens.cpp`

- Responsibility: define the `Token` sum type plus debug streaming.
- API: alternative aggregates `Word/Integer/Decimal/Symbol/Punctuation/Whitespace`;
  `using Token = std::variant<...>;`
  `std::ostream& operator<<(std::ostream&, const Token&);`.
- Contract: exactly one active alternative; `operator<<` total over all
  alternatives (enforced by `if constexpr` exhaustiveness).

### 4.4 `spoken/tokenizer.hpp` and `tokenizer.cpp`

- Responsibility: lexical analysis of input into tokens.
- API: `class Tokenizer { [[nodiscard]] std::vector<Token>`
  `tokenize(std::string_view) const; };` plus a free `tokenize(std::string_view)`.
- Contract: stateless; total over any input; whitespace preserved as `Whitespace`
  tokens; concatenating token source spans reproduces the input.
- Errors: `InvalidInputError` only when a numeric span fails to parse.

### 4.5 `spoken/number_converter.hpp` and `number_converter.cpp`

- Responsibility: integer and decimal to English words.
- API: `[[nodiscard]] std::string to_words(std::int64_t) const;`
  and `[[nodiscard]] std::string to_words(double) const;`.
- Contract: pure, deterministic, no global state; output rules per Section 6.1-6.2.
  Not `constexpr` in v1 (returns `std::string`).

### 4.6 `spoken/symbol_converter.hpp` and `symbol_converter.cpp`

- Responsibility: symbol glyph to spoken words.
- API: constructor builds the table; `[[nodiscard]] std::string`
  `to_words(std::string_view) const;`
  `[[nodiscard]] bool known(std::string_view) const noexcept;`.
- Contract: Rule of 0; known glyphs per the Section 6.3 table; unknown glyphs
  spelled per the fallback map.

### 4.7 `spoken/sentence_builder.hpp` and `sentence_builder.cpp`

- Responsibility: join spoken-per-token forms into the final sentence.
- API: `[[nodiscard]] std::string assemble(const std::vector<std::string>&) const;`.
- Contract: must implement the Section 6.4 spacing/capitalization/punctuation rules
  exactly (this is where the sketch is wrong — see DEF-2).

### 4.8 `spoken/conversion_pipeline.hpp` and `conversion_pipeline.cpp`

- Responsibility: orchestrate tokenize -> convert -> assemble.
- API: `class ConversionPipeline { ConversionPipeline();`
  `[[nodiscard]] std::string convert(std::string_view) const; };`.
- Contract: composition of the four converters (Rule of 0); thread-safe for
  concurrent `const` calls (no shared mutable state).

### 4.9 `spoken/tts/tts_engine.hpp`

- Responsibility: abstract TTS interface.
- API: `virtual ~TtsEngine(); virtual void speak(std::string_view)=0;`
  `virtual void to_wav(std::string_view, const std::string&)=0;`
  `[[nodiscard]] virtual std::string name() const =0;`.
- Contract: virtual destructor (polymorphic base); implementations may throw `TtsError`.

### 4.10 `spoken/tts/espeak_engine.*`

- Responsibility: real engine wrapping the eSpeak NG C API.
- API: `final` class, move-only (copy deleted, move `noexcept`), `set_voice`,
  `set_speed(noexcept)`, Pimpl via `std::unique_ptr<Impl>`.
- Contract: header must not include eSpeak headers (NFR-6); only compiled when
  `SPOKEN_HAVE_ESPEAK`; WAV synthesis must be cross-platform (see DEF-6).
- Errors: `TtsError` on init/voice/synthesis failure.

### 4.11 `spoken/tts/null_engine.*`

- Responsibility: no-audio engine writing spoken text to a sink.
- API: `using Sink = std::function<void(std::string_view)>; explicit NullEngine(Sink);`.
- Contract: throws `TtsError` if the sink is empty; `to_wav` writes a placeholder
  text file.

### 4.12 `spoken/tts/factory.hpp` and `factory.cpp`

- Responsibility: construct the configured engine.
- API: `enum class EngineKind { Auto, Espeak, Null }; struct EngineConfig {...};`
  `[[nodiscard]] std::unique_ptr<TtsEngine> make_engine(const EngineConfig&);`.
- Contract: Auto resolves to Espeak iff available, else Null; eSpeak availability
  check must be cross-platform (see DEF-7); `EngineConfig` must be constructed
  without designated initializers (NFR-1, DEF-8).
- Layout note: `src/tts/factory.cpp` is referenced by the build and provided in the
  sketch (7.19) but is missing from the project tree (3) and concept map (2). This
  spec mandates it exists (DEF-9).

---

## 5. Data Model

### 5.1 Token alternatives

- `Word`        : `std::string text`     — letters, `'`, `-` (example: apples).
- `Integer`     : `std::int64_t value`   — thousands separators stripped (42, 1,000).
- `Decimal`     : `double value`         — contains `.` (3.14).
- `Symbol`      : `std::string glyph`    — 1-2 char operators/symbols (`$`, `!=`, `->`).
- `Punctuation` : `char mark`            — one of `.!,?;:`.
- `Whitespace`  : `std::string spaces`   — preserved verbatim.

`using Token = std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;`

### 5.2 Configuration types

- `EngineConfig { EngineKind kind=Auto; std::string voice="en";`
  `int speed_wpm=175; std::string text_sink; }`.
- `Args` (CLI) — see Section 7.

---

## 6. Behavioural Specification (Normalization Rules)

This section is normative — tests assert against it.

### 6.1 Integers (FR-2)

- `0` -> "zero"; `21` -> "twenty-one"; `100` -> "one hundred";
  `115` -> "one hundred fifteen".
- Tens-and-ones are hyphenated ("forty-two"); hundreds are not followed by "and"
  ("one hundred fifteen", US style).
- Negatives are prefixed "negative " and the prefix MUST be preserved in the final
  string (the sketch loses it — DEF-3).
- Scale words: thousand, million, billion, trillion, quadrillion, quintillion.

### 6.2 Decimals (FR-3)

- `3.14` -> "three point one four"; `0.5` -> "zero point five";
  `-2.71` -> "negative two point seven one".
- Rule: integer part via Section 6.1; then " point"; then each fractional digit of
  the original literal spoken individually, with no trailing-zero padding and no
  spurious leading zero (the sketch's `snprintf("%.10f")` is wrong — DEF-4).
- Recommended implementation: operate on the original numeric substring from the
  tokenizer (preserve the lexeme), not a reformatted `double`, to avoid
  float-formatting artifacts.

### 6.3 Symbols (FR-4)

- Known table (normative subset): `$`->dollars, `%`->percent, `@`->at, `&`->and,
  `+`->plus, `-`->minus, `*`->times, `/`->divided by, `=`->equals,
  `!=`->not equals, `->`->arrow, `<`->less than, `>`->greater than.
- Unknown single chars are spelled: `#`->hash, `~`->tilde, backtick->backtick,
  `^`->caret, `|`->pipe, backslash->backslash.

### 6.4 Sentence assembly (FR-5) — normative spacing rules

Given the per-token spoken forms (which include whitespace tokens), `assemble` must:

1. Drop empty forms.
2. Join word/number/symbol forms with exactly one space.
3. Attach punctuation with no preceding space: "apples" + "." -> "apples."
   (the sketch produces "apples ." — DEF-2).
4. Collapse any internal whitespace runs to a single space; trim the ends.
5. Capitalize the first alphabetic character.
6. Ensure terminal punctuation: if the last non-space char is not one of `.?!`,
   append `.`.

Decision needed (Q-1): whitespace tokens vs. builder-driven spacing. The spec
recommends the builder owns spacing and the pipeline drops `Whitespace` tokens,
rather than preserving them and re-collapsing — this removes the double-spacing
class of bugs. See Section 13.

### 6.5 Currency (clarification)

The sketch's `pipeline_currency` test expects `$99.99` to become "ninety-nine
dollars ninety-nine cents", but the defined token grammar produces
`Symbol($) Decimal(99.99)` -> "dollars ninety-nine point nine nine". Currency
fusion (`$` + number -> "N dollars and M cents") is NOT in v1 scope (it is
Extension Exercise #2). v1 acceptance tests must use the grammar-accurate
expectation, or the feature must be promoted into scope. This contradiction must be
resolved before sign-off (DEF-5).

---

## 7. CLI Specification

### 7.1 Synopsis

```
spoken [options] [TEXT...]
spoken --file <path> [options]
echo "..." | spoken [options]
spoken --batch <dir> [options]
```

### 7.2 Options

- `-h, --help`            : print help, exit 0.
- `-f, --file <path>`     : read input from file.
- `-o, --out <path>`      : write spoken text (default stdout).
- `-w, --wav <path>`      : synthesize WAV.
- `--no-speak`            : suppress audio (speak is on by default).
- `--voice <name>`        : eSpeak voice (default `en`).
- `--speed <wpm>`         : speech rate (default 175).
- `-j, --jobs <N>`        : batch workers (default hardware concurrency).
- `--batch <dir>`         : process `*.txt` in dir.
- `--null-tts`            : force the null engine.
- `--verbose`             : per-record diagnostics.

### 7.3 Exit codes

|------|--------------------|
| Code | Meaning            |
|------|--------------------|
| 0    | success            |
| 1    | runtime / IO error |
| 2    | bad arguments      |
| 3    | TTS init failure   |
|------|--------------------|

### 7.4 Argument parser contract

- Returns `std::optional<Args>`; `std::nullopt` causes exit 2.
- Must not use `std::string_view::starts_with` (C++20). Use
  `arg.rfind('-', 0) == 0` or `arg.size() > 0 && arg[0] == '-'` (DEF-1).

---

## 8. Concurrency Model

- Unit of parallelism: one record (file) per
  `std::async(std::launch::async, ...)` task.
- Shared state:
  - `ConversionPipeline` — used `const` across threads; safe (no mutable state, NFR-4).
  - `TtsEngine` — a single shared instance is NOT thread-safe for `speak`/`to_wav`.
    Spec: in batch mode, conversion and file IO run in parallel, but engine calls
    MUST be serialized (a `std::mutex` around `speak`/`to_wav`, or per-task engines).
    The sketch shares one engine across tasks and calls `to_wav` concurrently —
    a data race (DEF-10).
  - Progress counter: `std::atomic<std::size_t>` (relaxed is sufficient for a counter).
- Ordering: the input file list is sorted (`std::sort`, optionally
  `std::execution::par`) for deterministic reporting (FR-10). Note that
  `std::execution::par` needs a parallel backend (libstdc++ -> TBB); the spec makes
  the parallel policy optional with a sequential fallback.
- TSan requirement: the batch path must be clean under ThreadSanitizer (NFR-3).

---

## 9. Build, Configuration & Portability

### 9.1 Toolchain matrix

- Linux   : GCC >= 9 / Clang >= 7 ; TTS via apt `libespeak-ng-dev` ; status primary.
- macOS   : AppleClang / Clang     ; TTS via brew `espeak-ng`       ; status supported.
- Windows : MSVC >= 19.15          ; TTS via vcpkg `espeak-ng`      ; status supported
  (the no-TTS path must work).

### 9.2 CMake contract

- `CMAKE_CXX_STANDARD 17`, `STANDARD_REQUIRED ON`, `EXTENSIONS OFF`.
- Options: `SPOKEN_BUILD_TESTS`, `SPOKEN_BUILD_EXAMPLES`,
  `SPOKEN_WARNINGS_AS_ERRORS`, `SPOKEN_WITH_ESPEAK`, `SPOKEN_SANITIZERS`.
- Targets: library `spoken`; CLI `spoken-cli`; tests `spoken_tests`;
  example `batch_convert`.
- eSpeak detected via `pkg_check_modules`; defines `SPOKEN_HAVE_ESPEAK=0/1`;
  `espeak_engine.cpp` compiled only when found.

### 9.3 Portability requirements (gaps in the sketch)

- `espeak_engine.cpp` uses `mkstemp`, `::close`, `/tmp`, and
  `std::system("... > /dev/null 2>&1")` — all POSIX-only, breaking the Windows
  claim (DEF-6).
- `factory.cpp` `espeak_available()` uses POSIX redirection (DEF-7).
- Spec: gate POSIX-only code behind `#ifdef`, or use
  `std::filesystem::temp_directory_path()` plus the eSpeak synthesis API instead of
  shelling out.

### 9.4 Sanitizers & CI

- ASan+UBSan build green for unit tests; a separate TSan build green for
  batch/concurrency.
- CI (GitHub Actions) builds both sanitizer configs and runs `ctest`.

---

## 10. Test Plan & Acceptance Criteria

### 10.1 Test levels

- Unit: tokenizer, number converter, symbol converter, sentence builder.
- Integration: `ConversionPipeline` end-to-end (text -> sentence).
- System (manual/CI): CLI single + batch, exit codes, WAV/null behavior.
- Sanitizer: all unit/integration under ASan+UBSan; batch under TSan.

### 10.2 Requirement to test traceability

- FR-1     -> `test_tokenizer.*` (words/numbers/symbols/multi-char op).
- FR-2     -> `test_number_converter` (small/negatives/thousands);
  DEF-3 must be fixed for negatives to pass.
- FR-3     -> `test_number_converter` (decimals); DEF-4 must be fixed.
- FR-4     -> `test_symbol_converter` (lookup + fallback).
- FR-5     -> `test_sentence_builder`; DEF-2 must be fixed for spacing.
- FR-6..9  -> system tests (CLI); add to CI.
- FR-10    -> system test (batch) + TSan; DEF-10 must be fixed.
- FR-11    -> factory test (add): Auto/Null/Espeak.
- FR-12    -> `examples/batch_convert` builds and links;
  fix the documented bug or keep it as a teaching note.
- FR-13    -> CLI exit-code tests (add).

### 10.3 Acceptance criteria (Definition of Done)

1. `cmake --build` succeeds on all three platforms with no warnings (NFR-2).
2. `ctest` green, including the corrected expectations from Section 6.
3. ASan+UBSan and TSan builds green in CI.
4. No C++20 constructs anywhere (Section 12 checklist all OK).
5. Public headers compile standalone and leak no third-party headers (NFR-6).
6. All Section 11 defects Resolved or explicitly Accepted-as-teaching with a note.

### 10.4 Missing-coverage gaps to add

- No test exercises the factory (`make_engine`) or the null-engine sink.
- No CLI/exit-code tests.
- `test_number_converter::compile_time` `static_assert`s a non-`constexpr`
  `std::string` result — it will not compile (DEF-11); convert to a runtime check
  or add a separate `constexpr` path.
- `tests/CMakeLists.txt` ties into currency expectations that conflict with the
  grammar (DEF-5).

---

## 11. Review Findings — Defect Register

Severity legend: S1 build/compile break; S2 wrong output / failing test;
S3 portability or race; S4 doc/consistency.

### 11.1 Defect status summary

|--------|-----|----------|
| ID     | Sev | Status   |
|--------|-----|----------|
| DEF-1  | S1  | Planned  |
| DEF-2  | S2  | Planned  |
| DEF-3  | S2  | Planned  |
| DEF-4  | S2  | Planned  |
| DEF-5  | S2  | Decision |
| DEF-6  | S3  | Planned  |
| DEF-7  | S3  | Planned  |
| DEF-8  | S1  | Planned  |
| DEF-9  | S4  | Doc fix  |
| DEF-10 | S3  | Planned  |
| DEF-11 | S1  | Planned  |
| DEF-12 | S4  | Doc fix  |
| DEF-13 | S4  | Teaching |
|--------|-----|----------|

### 11.2 Defect details

- DEF-1 (S1, sketch 7.22 `parse_args`): `std::string_view::starts_with` is C++20;
  it violates NFR-1 and will not compile in strict C++17.
  Fix: use `arg.rfind('-', 0) == 0` or `!arg.empty() && arg[0] == '-'`.

- DEF-2 (S2, sketch 7.12 `sentence_builder.cpp`): joining puts a space before
  punctuation ("apples ." not "apples."), and relies on re-collapsing preserved
  whitespace. Fails `pipeline_basic` and `pipeline_symbols`.
  Fix: implement the Section 6.4 rules (no space before `.,!?;:`; drop or normalize
  whitespace tokens).

- DEF-3 (S2, sketch 7.8 `to_words(int64)`): the chunk loop does
  `out = std::move(part)`, overwriting the "negative " prefix; `-1234` becomes
  "one thousand ..." with no "negative". Fails `negatives`.
  Fix: build the magnitude separately, then prepend "negative " after the loop.

- DEF-4 (S2, sketch 7.8 `to_words(double)`): `snprintf("%.10f", frac)` yields
  "0.1400000000"; the code iterates all digits including the leading 0 and trailing
  zeros, producing "three point zero one four zero ...". Fails `decimals`.
  Fix: speak fractional digits from the original lexeme; stop at the literal's last
  digit. Prefer passing the source substring from the tokenizer.

- DEF-5 (S2, Section 6 / sketch 7.29): `pipeline_currency` expects currency fusion
  ("ninety-nine dollars ninety-nine cents") not produced by the v1 grammar
  (`$` + Decimal). Spec contradiction.
  Fix: either (a) descope the test to grammar-accurate output, or (b) promote
  currency fusion into FR scope and implement it. Reviewer decides (Q-2).

- DEF-6 (S3, sketch 7.15 `espeak_engine.cpp`): POSIX-only `mkstemp`/`::close`/`/tmp`/
  `std::system(... 2>&1)`; missing includes (`<unistd.h>`, `<fstream>`, `<cstdio>`);
  breaks Windows.
  Fix: use `fs::temp_directory_path()`; gate POSIX code; or use the eSpeak
  synth-to-WAV API. Add the missing includes.

- DEF-7 (S3, sketch 7.19 `factory.cpp`): `espeak_available()` uses
  `std::system("espeak-ng --version > /dev/null 2>&1")` (POSIX redirection); also the
  Null branch uses `std::cout` but the file omits `<iostream>`.
  Fix: cross-platform detection (or rely on the CMake `SPOKEN_HAVE_ESPEAK` define);
  add `<iostream>`.

- DEF-8 (S1, sketch 7.30 and 9): `make_engine({.kind=..., .text_sink=...})` uses
  designated initializers (C++20); it will not compile in strict C++17.
  Fix: construct `EngineConfig` field-by-field, or add a builder/constructor.

- DEF-9 (S4, sketch 2 and 3): `src/tts/factory.cpp` is built (CMake 7.31) and
  specified (7.19) but absent from the file tree (3) and concept map (2).
  Fix: add it to the tree and concept map.

- DEF-10 (S3, sketch 7.22 batch branch): one shared `TtsEngine` has `to_wav`/`speak`
  called from concurrent `std::async` tasks — a data race (TSan failure) that
  contradicts the inline "we serialize" comment (no actual serialization).
  Fix: serialize engine calls with a `std::mutex`, or build one engine per task, or
  run TTS after the parallel join.

- DEF-11 (S1, sketch 7.25 `compile_time` test):
  `static_assert(NumberConverter{}.to_words(0) == "zero")` — `to_words` returns
  `std::string` (not `constexpr` in C++17); it will not compile.
  Fix: make it a runtime `REQUIRE_EQ`, or provide a separate `constexpr`
  `std::string_view` path for small values.

- DEF-12 (S4, sketch 10 Troubleshooting): "GCC 8 errors on `inline namespace v1`" is
  incorrect — inline namespaces are C++11. The real C++17 floor is driven by
  `if constexpr`, structured bindings, and `optional`.
  Fix: correct the troubleshooting note.

- DEF-13 (S4, sketch 7.30 example): `examples/batch_convert.cpp` ships a deliberate
  capture-by-reference bug. Acceptable as a teaching artifact but it must be clearly
  fenced, must still compile (it does), and must not be wired into pass/fail CI.
  Fix: keep it, but label it and exclude it from correctness gates.

Reviewer triage: S1/S2/S3 (DEF-1, 2, 3, 4, 6, 7, 8, 10, 11) block acceptance.
S4 are documentation fixes. DEF-5 needs a product decision (Q-2).

---

## 12. C++17 Conformance Checklist

- OK? | Item
- NO  : `std::string_view::starts_with` is C++20 (DEF-1).
- NO  : designated initializers are C++20 (DEF-8).
- OK  : no `consteval` / ranges / concepts.
- OK  : `if constexpr`, structured bindings, `optional`/`variant`/`filesystem` used.
- OK  : `inline` variables and `inline namespace` (C++11) used.
- OK  : build is `-std=c++17` strict (`EXTENSIONS OFF`); the code must comply too.

---

## 13. Open Questions / Decisions Needed

- Q-1 (spacing model): drop `Whitespace` tokens and let `SentenceBuilder` own all
  spacing (spec recommendation), or keep preserving whitespace and fix collapse?
  Recommendation: drop whitespace tokens. Affects the DEF-2 fix.
- Q-2 (currency scope): is `$` + number -> "... dollars and ... cents" in v1
  (promote to FR) or an extension (descope the test)? Blocks DEF-5.
- Q-3 (WAV strategy): shell out to the `espeak-ng` CLI (simple, fragile, POSIX) vs.
  link the eSpeak synthesis API (portable, more code)?
  Recommendation: synthesis API; CLI fallback only on POSIX.
- Q-4 (decimal source): pass the original numeric lexeme through the
  `Decimal`/`Integer` tokens (add a `std::string lexeme`) to make Section 6.2 exact?
  Recommendation: yes.
- Q-5 (parallel policy): require TBB for `std::execution::par`, or keep `std::sort`
  sequential and label parallel STL as optional?
  Recommendation: optional with fallback.

---

## 14. Sign-off Checklist

- [ ] Section 2 requirements approved (or amended).
- [ ] Section 6 normalization rules approved as the test oracle.
- [ ] Q-1 through Q-5 decided and recorded.
- [ ] DEF-1, DEF-8, DEF-11 (compile breaks) scheduled.
- [ ] DEF-2, DEF-3, DEF-4 (output bugs) scheduled with corrected test expectations.
- [ ] DEF-6, DEF-7, DEF-10 (portability/race) scheduled.
- [ ] DEF-5 product decision recorded.
- [ ] CI matrix (Linux/macOS/Windows x {ASan+UBSan, TSan}) agreed.
- [ ] Acceptance criteria (Section 10.3) accepted as the Definition of Done.

On sign-off, the sketch (`001_the_project_sketch.md`) becomes the implementation
reference, corrected against this spec. A follow-up document carrying the applied
fixes should be added as a new file (per the project rules).
