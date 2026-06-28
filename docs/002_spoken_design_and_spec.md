# `spoken` — Design & Specification (for Review)

> **Status:** 📋 DRAFT FOR REVIEW
> **Source artifact reviewed:** [`001_the_project_sketch.md`](001_the_project_sketch.md) (the implementation sketch)
> **Relationship:** This document is the *authoritative specification*. Where the
> sketch and this spec disagree, **this spec wins** and the sketch must be
> corrected. §11 (Defect Register) lists every known divergence.
> **Reviewer action:** Approve §2–§10 as the contract, then triage §11 defects
> before any code is committed.

---

## Table of Contents

1. [Purpose & Scope](#1-purpose--scope)
2. [Requirements](#2-requirements)
3. [Architecture Overview](#3-architecture-overview)
4. [Module Specifications & Interface Contracts](#4-module-specifications--interface-contracts)
5. [Data Model](#5-data-model)
6. [Behavioural Specification (Normalization Rules)](#6-behavioural-specification-normalization-rules)
7. [CLI Specification](#7-cli-specification)
8. [Concurrency Model](#8-concurrency-model)
9. [Build, Configuration & Portability](#9-build-configuration--portability)
10. [Test Plan & Acceptance Criteria](#10-test-plan--acceptance-criteria)
11. [Review Findings — Defect Register](#11-review-findings--defect-register)
12. [C++17 Conformance Checklist](#12-c17-conformance-checklist)
13. [Open Questions / Decisions Needed](#13-open-questions--decisions-needed)
14. [Sign-off Checklist](#14-sign-off-checklist)

---

## 1. Purpose & Scope

### 1.1 Product summary
`spoken` is a C++17 command-line tool and reusable library that converts a line of
mixed text (words, numbers, symbols, punctuation) into a natural spoken-English
sentence, and optionally renders that sentence to audio (playback or WAV) via an
offline, open-source text-to-speech (TTS) engine.

### 1.2 Goals
- **G1 — Pedagogical:** exercise every load-bearing Tier 4 and Tier 5 concept from
  [`000_cpp17-reference.md`](000_cpp17-reference.md) in one coherent, real application.
- **G2 — Correct:** produce deterministic, testable text output for a defined input grammar.
- **G3 — Portable:** build and pass all tests on Linux, macOS, and Windows with GCC ≥ 9,
  Clang ≥ 7, or MSVC ≥ 19.15, **under `-std=c++17` strictly** (no GNU/C++20 extensions).
- **G4 — Degradable:** build and run with full test coverage **without** any TTS library
  installed (null engine).

### 1.3 Non-goals
- Not a general NLP/grammar engine; the input grammar is the closed set defined in §6.
- Not a real-time/streaming audio mixer (those are extension exercises).
- Not internationalized in v1 (English only; locale is an extension).

### 1.4 Definitions
| Term | Meaning |
|---|---|
| Token | One lexical unit produced by the tokenizer (see §5.1). |
| Record | One input line/file processed into one spoken sentence. |
| Engine | A `TtsEngine` implementation (real `espeak` or `null`). |
| Null engine | A `TtsEngine` that emits spoken *text* to a sink instead of audio. |

---

## 2. Requirements

### 2.1 Functional requirements

| ID | Requirement | Priority |
|---|---|---|
| FR-1 | Tokenize input into Word / Integer / Decimal / Symbol / Punctuation / Whitespace tokens. | MUST |
| FR-2 | Convert integers (incl. negatives, up to ±9.2e18) to English words. | MUST |
| FR-3 | Convert decimals to English ("3.14" → "three point one four"). | MUST |
| FR-4 | Convert known symbols via a lookup table; spell unknown symbols by name. | MUST |
| FR-5 | Assemble per-token spoken forms into one capitalized, terminally-punctuated sentence with correct spacing (no space before `.,!?;:`). | MUST |
| FR-6 | Read input from positional args, `--file`, or stdin. | MUST |
| FR-7 | Write spoken text to stdout or `--out <path>`. | MUST |
| FR-8 | Speak aloud via TTS engine unless `--no-speak`/`--null-tts`. | SHOULD |
| FR-9 | Synthesize WAV via `--wav <path>`. | SHOULD |
| FR-10 | Batch-process every `*.txt` in `--batch <dir>` concurrently, deterministic ordering of report. | SHOULD |
| FR-11 | Select engine via factory (`Auto`/`Espeak`/`Null`); fall back to null when eSpeak unavailable. | MUST |
| FR-12 | Expose the conversion pipeline + factory as a linkable library API. | MUST |
| FR-13 | Report errors via a typed exception hierarchy; CLI maps them to exit codes. | MUST |

### 2.2 Non-functional requirements

| ID | Requirement |
|---|---|
| NFR-1 | **C++17 only.** No C++20 features (`starts_with`, designated initializers, `consteval`, ranges) and no compiler extensions. |
| NFR-2 | Builds clean under `-Wall -Wextra -pedantic` (and `/W4 /permissive-` on MSVC). |
| NFR-3 | All unit tests pass under ASan+UBSan; concurrency tests pass under TSan. |
| NFR-4 | Library has zero global mutable state; converters are value types (Rule of 0). |
| NFR-5 | Pure conversion functions are deterministic and locale-independent (`"C"` semantics). |
| NFR-6 | Public headers must not leak third-party C headers (eSpeak hidden behind Pimpl). |
| NFR-7 | CLI exit codes: `0` success, `1` runtime error, `2` bad args, `3` TTS init failure. |

---

## 3. Architecture Overview

### 3.1 Layered component view

```
┌──────────────────────────────────────────────────────────────┐
│  apps/spoken-cli              examples/batch_convert           │  Presentation
│  (arg parsing, IO, batch orchestration)                        │
└───────────────┬───────────────────────────┬──────────────────┘
                │ depends on                 │ depends on
                ▼                            ▼
┌──────────────────────────────┐  ┌──────────────────────────────┐
│  ConversionPipeline          │  │  tts::TtsEngine (interface)   │  Domain
│   ├─ Tokenizer               │  │   ├─ EspeakEngine (Pimpl)     │
│   ├─ NumberConverter         │  │   └─ NullEngine (sink)        │
│   ├─ SymbolConverter         │  │  tts::make_engine() (factory) │
│   └─ SentenceBuilder         │  └──────────────────────────────┘
└──────────────────────────────┘
                │
                ▼
┌──────────────────────────────────────────────────────────────┐
│  tokens (variant)   error (exception hierarchy)   version       │  Foundation
└──────────────────────────────────────────────────────────────┘
```

**Dependency rule:** arrows point *downward only*. Presentation depends on Domain
via interfaces (`TtsEngine`) — Dependency Inversion. No layer reaches up.

### 3.2 Data flow (single record)

```
raw text ─▶ Tokenizer ─▶ vector<Token>
                              │  std::transform + std::visit
                              ▼
                    vector<string> (spoken-per-token)
                              │  SentenceBuilder::assemble
                              ▼
                       spoken sentence ─┬─▶ stdout / --out file
                                        └─▶ TtsEngine::speak | to_wav
```

---

## 4. Module Specifications & Interface Contracts

Each module lists: **Responsibility**, **Public API (signatures)**, **Invariants /
Contracts**, **Errors**. Implementation lives in the sketch; this section is the
contract the implementation must satisfy.

### 4.1 `spoken/version.hpp`
- **Responsibility:** compile-time version constants.
- **API:** `inline constexpr std::uint32_t version_{major,minor,patch}; inline constexpr const char* version_string;`
- **Contract:** header-only, `inline constexpr` (ODR-safe across TUs). `inline namespace v1` permitted (C++11 feature).

### 4.2 `spoken/error.hpp`
- **Responsibility:** typed error hierarchy.
- **API:** `SpokenError : std::runtime_error` (root); `InvalidInputError : SpokenError`; `TtsError : SpokenError`; inheriting constructors via `using`.
- **Contract:** all library-thrown exceptions derive from `SpokenError`. `what()` is human-readable, non-empty.

### 4.3 `spoken/tokens.hpp` / `tokens.cpp`
- **Responsibility:** define `Token` sum type + debug streaming.
- **API:** alternative aggregates `Word/Integer/Decimal/Symbol/Punctuation/Whitespace`; `using Token = std::variant<...>;` `std::ostream& operator<<(std::ostream&, const Token&);`
- **Contract:** exactly one active alternative; `operator<<` total over all alternatives (enforced by `if constexpr` exhaustiveness).

### 4.4 `spoken/tokenizer.hpp` / `tokenizer.cpp`
- **Responsibility:** lexical analysis of input → tokens.
- **API:** `class Tokenizer { [[nodiscard]] std::vector<Token> tokenize(std::string_view) const; };` plus free `tokenize(std::string_view)`.
- **Contract:** stateless; total over any input; whitespace preserved as `Whitespace` tokens; concatenating token source spans reproduces the input.
- **Errors:** `InvalidInputError` only when a numeric span fails to parse.

### 4.5 `spoken/number_converter.hpp` / `number_converter.cpp`
- **Responsibility:** integer & decimal → English words.
- **API:** `[[nodiscard]] std::string to_words(std::int64_t) const;` `[[nodiscard]] std::string to_words(double) const;`
- **Contract:** pure, deterministic, no allocation of global state; output rules per §6.1–6.2. **Not** `constexpr` in v1 (returns `std::string`).

### 4.6 `spoken/symbol_converter.hpp` / `symbol_converter.cpp`
- **Responsibility:** symbol glyph → spoken words.
- **API:** ctor builds table; `[[nodiscard]] std::string to_words(std::string_view) const;` `[[nodiscard]] bool known(std::string_view) const noexcept;`
- **Contract:** Rule of 0; known glyphs per §6.3 table; unknown glyphs spelled per fallback map.

### 4.7 `spoken/sentence_builder.hpp` / `sentence_builder.cpp`
- **Responsibility:** join spoken-per-token forms → final sentence.
- **API:** `[[nodiscard]] std::string assemble(const std::vector<std::string>&) const;`
- **Contract:** must implement §6.4 spacing/capitalization/punctuation rules exactly (this is where the sketch is wrong — see DEF-2).

### 4.8 `spoken/conversion_pipeline.hpp` / `conversion_pipeline.cpp`
- **Responsibility:** orchestrate tokenize → convert → assemble.
- **API:** `class ConversionPipeline { ConversionPipeline(); [[nodiscard]] std::string convert(std::string_view) const; };`
- **Contract:** composition of the four converters (Rule of 0); thread-safe for concurrent `const` calls (no shared mutable state).

### 4.9 `spoken/tts/tts_engine.hpp`
- **Responsibility:** abstract TTS interface.
- **API:** `virtual ~TtsEngine(); virtual void speak(std::string_view)=0; virtual void to_wav(std::string_view, const std::string&)=0; [[nodiscard]] virtual std::string name() const =0;`
- **Contract:** virtual destructor (polymorphic base); implementations may throw `TtsError`.

### 4.10 `spoken/tts/espeak_engine.*`
- **Responsibility:** real engine wrapping eSpeak NG C API.
- **API:** `final` class, move-only (copy deleted, move `noexcept`), `set_voice`, `set_speed(noexcept)`, Pimpl (`std::unique_ptr<Impl>`).
- **Contract:** header must not include eSpeak headers (NFR-6); only compiled when `SPOKEN_HAVE_ESPEAK`. WAV synthesis must be cross-platform (see DEF-6).
- **Errors:** `TtsError` on init/voice/synthesis failure.

### 4.11 `spoken/tts/null_engine.*`
- **Responsibility:** no-audio engine writing spoken text to a sink.
- **API:** `using Sink = std::function<void(std::string_view)>; explicit NullEngine(Sink);`
- **Contract:** throws `TtsError` if sink is empty; `to_wav` writes a placeholder text file.

### 4.12 `spoken/tts/factory.hpp` / `factory.cpp`
- **Responsibility:** construct the configured engine.
- **API:** `enum class EngineKind { Auto, Espeak, Null }; struct EngineConfig {…}; [[nodiscard]] std::unique_ptr<TtsEngine> make_engine(const EngineConfig&);`
- **Contract:** `Auto` resolves to Espeak iff available, else Null. eSpeak availability check must be cross-platform (see DEF-7). **`EngineConfig` must be constructed without designated initializers** (NFR-1, DEF-8).

> ⚠️ **Layout note:** `src/tts/factory.cpp` is referenced by the build (CMake) and
> provided in the sketch (§7.19) but is **missing from the project tree (§3) and the
> concept map (§2)**. Spec mandates it exists. (DEF-9)

---

## 5. Data Model

### 5.1 Token alternatives

| Alternative | Field | Example source | Notes |
|---|---|---|---|
| `Word` | `std::string text` | `apples` | letters, `'`, `-` |
| `Integer` | `std::int64_t value` | `42`, `1,000` | thousands separators stripped |
| `Decimal` | `double value` | `3.14` | contains `.` |
| `Symbol` | `std::string glyph` | `$`, `!=`, `->` | 1–2 char operators/symbols |
| `Punctuation` | `char mark` | `.`, `!`, `?` | one of `.!,?;:` |
| `Whitespace` | `std::string spaces` | `"  "` | preserved verbatim |

`using Token = std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;`

### 5.2 Configuration types
- `EngineConfig { EngineKind kind=Auto; std::string voice="en"; int speed_wpm=175; std::string text_sink; }`
- `Args` (CLI) — see §7.

---

## 6. Behavioural Specification (Normalization Rules)

This section is **normative** — tests assert against it. Pin down ambiguous cases here.

### 6.1 Integers (FR-2)
- `0` → `"zero"`; `21` → `"twenty-one"`; `100` → `"one hundred"`; `115` → `"one hundred fifteen"`.
- Tens-and-ones hyphenated: `forty-two`. Hundreds **not** followed by "and": `one hundred fifteen` (US style).
- Negatives prefixed `"negative "` and the prefix **must be preserved** in the final string (sketch loses it — DEF-3).
- Scale words: thousand, million, billion, trillion, quadrillion, quintillion.

### 6.2 Decimals (FR-3)
- `3.14` → `"three point one four"`; `0.5` → `"zero point five"`; `-2.71` → `"negative two point seven one"`.
- **Rule:** integer part via §6.1; then `" point"`; then each fractional digit **of the original literal** spoken individually, **no trailing-zero padding and no spurious leading zero** (sketch's `snprintf("%.10f")` is wrong — DEF-4).
- Recommended implementation: operate on the original numeric **substring** from the tokenizer (preserve the lexeme), not a reformatted `double`, to avoid float-formatting artifacts.

### 6.3 Symbols (FR-4)
- Known table (subset, normative): `$`→dollars, `%`→percent, `@`→at, `&`→and, `+`→plus, `-`→minus, `*`→times, `/`→divided by, `=`→equals, `!=`→not equals, `->`→arrow, `<`→less than, `>`→greater than.
- Unknown single chars spelled: `#`→hash, `~`→tilde, `` ` ``→backtick, `^`→caret, `|`→pipe, `\`→backslash.

### 6.4 Sentence assembly (FR-5) — **normative spacing rules**
Given the per-token spoken forms (which include whitespace tokens), `assemble` must:
1. Drop empty forms.
2. Join word/number/symbol forms with exactly **one** space.
3. **Attach punctuation with no preceding space**: `apples` + `.` → `apples.` (the sketch produces `apples .` — DEF-2).
4. Collapse any internal whitespace runs to a single space; trim ends.
5. Capitalize the first alphabetic character.
6. Ensure terminal punctuation: if the last non-space char is not one of `.?!`, append `.`.

> **Decision needed (Q-1):** whitespace tokens vs. builder-driven spacing. Spec
> recommends the builder owns spacing and the pipeline **drops `Whitespace` tokens**,
> rather than preserving them and re-collapsing — this removes the double-spacing
> class of bugs. See §13.

### 6.5 Currency (clarification)
The sketch's `pipeline_currency` test expects `$99.99` → "ninety-nine dollars ninety-nine cents",
but the defined token grammar produces `Symbol($) Decimal(99.99)` → "dollars ninety-nine point nine nine".
**Currency fusion (`$` + number → "N dollars and M cents") is NOT in v1 scope** (it's
Extension Exercise #2). v1 acceptance tests must use the grammar-accurate expectation, or
the feature must be promoted into scope. **This contradiction must be resolved before sign-off** (DEF-5).

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
| Flag | Arg | Default | Meaning |
|---|---|---|---|
| `-h, --help` | — | — | Print help, exit 0 |
| `-f, --file` | path | — | Read input from file |
| `-o, --out` | path | stdout | Write spoken text |
| `-w, --wav` | path | — | Synthesize WAV |
| `--no-speak` | — | speak on | Suppress audio |
| `--voice` | name | `en` | eSpeak voice |
| `--speed` | wpm | `175` | Speech rate |
| `-j, --jobs` | N | hw concurrency | Batch workers |
| `--batch` | dir | — | Process `*.txt` in dir |
| `--null-tts` | — | off | Force null engine |
| `--verbose` | — | off | Per-record diagnostics |

### 7.3 Exit codes
`0` ok · `1` runtime/IO error · `2` bad arguments · `3` TTS init failure (per NFR-7).

### 7.4 Argument parser contract
- Returns `std::optional<Args>`; `std::nullopt` ⇒ exit 2.
- **Must not use `std::string_view::starts_with`** (C++20). Use `arg.size()>0 && arg[0]=='-'` or `arg.rfind("-",0)==0` (DEF-1).

---

## 8. Concurrency Model

- **Unit of parallelism:** one record (file) per `std::async(std::launch::async, …)` task.
- **Shared state:**
  - `ConversionPipeline` — used `const` across threads; safe (no mutable state, NFR-4).
  - `TtsEngine` — **single shared instance is NOT thread-safe** for `speak`/`to_wav`. Spec: in batch mode, conversion + file IO run in parallel; **engine calls must be serialized** (a `std::mutex` around `speak`/`to_wav`, or per-task engines). The sketch shares one engine across tasks and calls `to_wav` concurrently — **data race** (DEF-10).
  - Progress counter: `std::atomic<std::size_t>` (relaxed is sufficient for a counter).
- **Ordering:** input file list sorted (`std::sort`, optionally `std::execution::par`) for deterministic reporting (FR-10). Note `std::execution::par` requires a parallel backend (libstdc++ → TBB); spec makes parallel policy **optional** with sequential fallback.
- **TSan requirement:** batch path must be clean under ThreadSanitizer (NFR-3).

---

## 9. Build, Configuration & Portability

### 9.1 Toolchain matrix
| Platform | Compiler | TTS source | Status |
|---|---|---|---|
| Linux | GCC ≥ 9 / Clang ≥ 7 | apt `libespeak-ng-dev` | primary |
| macOS | AppleClang / Clang | brew `espeak-ng` | supported |
| Windows | MSVC ≥ 19.15 | vcpkg `espeak-ng` | supported (no-TTS path must work) |

### 9.2 CMake contract
- `CMAKE_CXX_STANDARD 17`, `STANDARD_REQUIRED ON`, `EXTENSIONS OFF`.
- Options: `SPOKEN_BUILD_TESTS`, `SPOKEN_BUILD_EXAMPLES`, `SPOKEN_WARNINGS_AS_ERRORS`, `SPOKEN_WITH_ESPEAK`, `SPOKEN_SANITIZERS`.
- Library target `spoken`; CLI `spoken-cli`; tests `spoken_tests`; example `batch_convert`.
- eSpeak detected via `pkg_check_modules`; defines `SPOKEN_HAVE_ESPEAK=0/1`. `espeak_engine.cpp` compiled only when found.

### 9.3 Portability requirements (gaps in sketch)
- `espeak_engine.cpp` uses `mkstemp`, `::close`, `/tmp`, `std::system("… > /dev/null 2>&1")` — **POSIX-only**, breaks the Windows claim (DEF-6).
- `factory.cpp` `espeak_available()` uses POSIX redirection (DEF-7).
- Spec: gate POSIX-only code behind `#ifdef`, or use `std::filesystem::temp_directory_path()` + the eSpeak synthesis API instead of shelling out.

### 9.4 Sanitizers & CI
- ASan+UBSan build green for unit tests; separate TSan build green for batch/concurrency.
- CI (GitHub Actions) builds both sanitizer configs and runs `ctest`.

---

## 10. Test Plan & Acceptance Criteria

### 10.1 Test levels
- **Unit:** tokenizer, number converter, symbol converter, sentence builder.
- **Integration:** `ConversionPipeline` end-to-end (text → sentence).
- **System (manual/CI):** CLI single + batch, exit codes, WAV/null behavior.
- **Sanitizer:** all unit/integration under ASan+UBSan; batch under TSan.

### 10.2 Requirement → test traceability

| Req | Covered by | Notes |
|---|---|---|
| FR-1 | `test_tokenizer.*` | words/numbers/symbols/multi-char op |
| FR-2 | `test_number_converter` (small/negatives/thousands) | DEF-3 must be fixed for negatives to pass |
| FR-3 | `test_number_converter` (decimals) | DEF-4 must be fixed |
| FR-4 | `test_symbol_converter` | lookup + fallback |
| FR-5 | `test_sentence_builder` | DEF-2 must be fixed for spacing |
| FR-6..9 | system tests (CLI) | add to CI |
| FR-10 | system test (batch) + TSan | DEF-10 must be fixed |
| FR-11 | factory test (add) | Auto/Null/Espeak |
| FR-12 | `examples/batch_convert` builds & links | fix documented bug or keep as teaching note |
| FR-13 | CLI exit-code tests (add) | |

### 10.3 Acceptance criteria (Definition of Done)
1. `cmake --build` succeeds on all three platforms with no warnings (NFR-2).
2. `ctest` green, including the corrected expectations from §6.
3. ASan+UBSan and TSan builds green in CI.
4. No C++20 constructs anywhere (§12 checklist all ✅).
5. Public headers compile standalone and leak no third-party headers (NFR-6).
6. §11 defects all Resolved or explicitly Accepted-as-teaching with a written note.

### 10.4 Missing-coverage gaps to add
- No test exercises the **factory** (`make_engine`) or **null engine** sink.
- No CLI/exit-code tests.
- `test_number_converter::compile_time` `static_assert`s a non-`constexpr` `std::string` result — **will not compile** (DEF-11); convert to a runtime check or make a separate `constexpr` path.
- `tests/CMakeLists.txt` does not list `test_pipeline_e2e.cpp`'s dependencies on currency expectations (DEF-5).

---

## 11. Review Findings — Defect Register

Severity: **S1** build/compile break · **S2** test will fail / wrong output · **S3** portability/race · **S4** doc/consistency.

| ID | Sev | Location (sketch §) | Finding | Required fix |
|---|---|---|---|---|
| DEF-1 | S1 | §7.22 `parse_args` (`arg.starts_with("-")`) | `std::string_view::starts_with` is **C++20**; violates NFR-1 and won't compile in strict C++17. | Use `arg.rfind('-',0)==0` or `!arg.empty() && arg[0]=='-'`. |
| DEF-2 | S2 | §7.12 `sentence_builder.cpp` | Joining puts a space before punctuation → "apples ." not "apples."; also relies on re-collapsing preserved whitespace. Fails `pipeline_basic`, `pipeline_symbols`. | Implement §6.4 rules: no space before `.,!?;:`; drop or normalize whitespace tokens. |
| DEF-3 | S2 | §7.8 `to_words(int64)` | Chunk loop does `out = std::move(part)`, **overwriting the "negative " prefix**. `-1234` → "one thousand…" (no "negative"). Fails `negatives`. | Build magnitude separately; prepend `"negative "` after the loop. |
| DEF-4 | S2 | §7.8 `to_words(double)` | `snprintf("%.10f", frac)` yields `"0.1400000000"`; code iterates **all** digits incl. leading `0` and trailing zeros → "three point zero one four zero…". Fails `decimals`. | Speak fractional digits from the **original lexeme**; stop at the literal's last digit. Prefer passing the source substring from the tokenizer. |
| DEF-5 | S2 | §6 / §7.29 | `pipeline_currency` expects currency fusion ("ninety-nine dollars ninety-nine cents") not produced by the v1 grammar (`$`+Decimal). Spec contradiction. | Either (a) descope test to grammar-accurate output, or (b) promote currency fusion into FR scope and implement. Reviewer decides (Q-2). |
| DEF-6 | S3 | §7.15 `espeak_engine.cpp` | POSIX-only `mkstemp`/`::close`/`/tmp`/`std::system(... 2>&1)`; missing includes (`<unistd.h>`, `<fstream>`, `<cstdio>`); breaks Windows. | Use `fs::temp_directory_path()`; gate POSIX code; or use eSpeak synth-to-WAV API. Add missing includes. |
| DEF-7 | S3 | §7.19 `factory.cpp` `espeak_available()` | `std::system("espeak-ng --version > /dev/null 2>&1")` is POSIX redirection; also `make_engine` Null branch uses `std::cout` but file omits `<iostream>`. | Cross-platform detection (or rely on CMake `SPOKEN_HAVE_ESPEAK`); add `<iostream>`. |
| DEF-8 | S1 | §7.30, §9 | `make_engine({.kind=…, .text_sink=…})` uses **designated initializers** (C++20). Won't compile in strict C++17. | Construct `EngineConfig` field-by-field, or add a builder/ctor. |
| DEF-9 | S4 | §2, §3 | `src/tts/factory.cpp` is built (CMake §7.31) and specified (§7.19) but **absent from the file tree (§3) and concept map (§2)**. | Add it to the tree and concept map. |
| DEF-10 | S3 | §7.22 batch branch | One shared `TtsEngine` has `to_wav`/`speak` called from concurrent `std::async` tasks → **data race** (TSan failure), contradicts the inline "we serialize" comment (no actual serialization). | Serialize engine calls with a `std::mutex`, or build one engine per task, or run TTS after the parallel join. |
| DEF-11 | S1 | §7.25 `compile_time` test | `static_assert(NumberConverter{}.to_words(0) == "zero")` — `to_words` returns `std::string` (not `constexpr` in C++17); won't compile. | Make it a runtime `REQUIRE_EQ`, or provide a separate `constexpr` `std::string_view` path for small values. |
| DEF-12 | S4 | §10 Troubleshooting | "GCC 8 errors on `inline namespace v1`" is incorrect — inline namespaces are C++11. The real C++17 floor is driven by `if constexpr`/structured bindings/`optional`. | Correct the troubleshooting note. |
| DEF-13 | S4 | §7.30 example | `examples/batch_convert.cpp` ships a deliberate capture-by-reference bug. Acceptable **as a teaching artifact** but must be clearly fenced and must still **compile** (it does) and not be wired into pass/fail CI. | Keep, but label and exclude from correctness gates. |

> **Reviewer triage:** S1/S2/S3 (DEF-1,2,3,4,6,7,8,10,11) **block** acceptance.
> S4 are documentation fixes. DEF-5 needs a product decision (Q-2).

---

## 12. C++17 Conformance Checklist

| Item | Rule | Status in sketch |
|---|---|---|
| No `std::string_view::starts_with` | C++20 | ❌ DEF-1 |
| No designated initializers | C++20 | ❌ DEF-8 |
| No `consteval` / ranges / concepts | C++20 | ✅ |
| `if constexpr`, structured bindings, `optional`/`variant`, `filesystem` | C++17 | ✅ used correctly |
| `inline` variables / `inline namespace` | C++17 / C++11 | ✅ |
| Build is `-std=c++17` strict (`EXTENSIONS OFF`) | NFR-1 | ✅ (CMake) — but code must comply |

---

## 13. Open Questions / Decisions Needed

- **Q-1 (spacing model):** Drop `Whitespace` tokens and let `SentenceBuilder` own all
  spacing (spec recommendation), **or** keep preserving whitespace and fix collapse?
  *Recommendation: drop whitespace tokens.* Affects DEF-2 fix.
- **Q-2 (currency scope):** Is `$`+number → "… dollars and … cents" **in v1** (promote to FR)
  or an extension (descope the test)? Blocks DEF-5.
- **Q-3 (WAV strategy):** Shell out to `espeak-ng` CLI (simple, fragile, POSIX) vs. link the
  eSpeak synthesis API (portable, more code)? *Recommendation: synthesis API; CLI fallback only on POSIX.*
- **Q-4 (decimal source):** Pass the original numeric lexeme through the `Decimal`/`Integer`
  tokens (add a `std::string lexeme`) to make §6.2 exact? *Recommendation: yes.*
- **Q-5 (parallel policy):** Require TBB for `std::execution::par`, or keep `std::sort`
  sequential and label parallel STL as optional? *Recommendation: optional with fallback.*

---

## 14. Sign-off Checklist

- [ ] §2 requirements approved (or amended).
- [ ] §6 normalization rules approved as test oracle.
- [ ] Q-1…Q-5 decided and recorded.
- [ ] DEF-1, DEF-8, DEF-11 (compile breaks) scheduled.
- [ ] DEF-2, DEF-3, DEF-4 (output bugs) scheduled with corrected test expectations.
- [ ] DEF-6, DEF-7, DEF-10 (portability/race) scheduled.
- [ ] DEF-5 product decision recorded.
- [ ] CI matrix (Linux/macOS/Windows × {ASan+UBSan, TSan}) agreed.
- [ ] Acceptance criteria (§10.3) accepted as Definition of Done.

> On sign-off, the sketch ([`001_the_project_sketch.md`](001_the_project_sketch.md)) becomes the
> implementation reference, corrected against this spec. A follow-up doc
> `003_spoken_implementation.md` (or the corrected project itself) should carry the fixes.
