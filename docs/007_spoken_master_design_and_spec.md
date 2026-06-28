# `spoken` — Master Design & Specification (CANONICAL)

> Status: CANONICAL. This document supersedes the spec drafts and all rating
> renditions for the purpose of building `spoken`. It consolidates the spec, resolves
> the open questions, folds in the accepted review suggestions, and adopts the project
> documentation rules surfaced by the review chain.
>
> Authoring compliance: `_mydocs/002_rules.md` (lines <= 120 chars, no tabs, no
> GitHub-style pipe-bordered tables except space-aligned `|---|` tables).

---

## 0. Provenance & Document Lineage

This master spec is the synthesis of the following artifacts. It does not modify them
(append-only / new-file rule); it consolidates and decides.

```
Ancestry (this is the canonical synthesis):
   reference  000_cpp17-reference.md            the C++17 reference
   sketch     001_the_project_sketch.md         the implementation walkthrough
   spec       002_spoken_design_and_spec.md     the spec (original)
   spec'      003_spoken_design_and_spec.md     the spec (rules-compliant rendition)
   rating     005_spoken_spec_rating.md         external review of the spec (8.7/8.9)
   re-rating  006_spoken_review_rerating.md      review of the rating (9.1)
   re-re      (007 input) review of the re-rating (9.3)
   MASTER     007_spoken_master_design_and_spec.md   <- this document
```

What this document does with its inputs:

- Carries the full spec substance from `002`/`003`.
- Resolves the five open questions (Section 13) to their recommended defaults.
- Applies review suggestions S1, S2, S3, S4, S5, S6, S7, S8, S10 and structural edits
  A, B, C (from `005`), and adopts the project-rule proposals M1/I4 (from `006`/`007
  input`).
- Records defect status and a "where each defect is fixed" mapping.

Scoring note: per the review chain's R1/R6 findings, ratings are tracked separately
and are not restated here (this is a spec, not a rating). The honest scoring
convention adopted by the project is in Section 16.

---

## 1. Project Documentation Rules (Adopted)

These extend `_mydocs/002_rules.md`. They exist to stop the rendition tree from
drifting (raised repeatedly across the review chain).

- R-Rendition-1: a rules-compliant rendition reproduces its source verbatim in
  meaning. Issues found in the source are tracked against the source, not silently
  fixed in the rendition. A rendition may flag them in a closing note.
- R-Naming-1: source artifacts keep their numeric prefix in their original location;
  any future rendition is a new file with its own sequential number and a clear name
  that states both its source file and its kind. No two files share a numeric prefix
  with different meanings.
- R-Canonical-1: exactly one document is CANONICAL for building `spoken` — this one.
  Superseding it requires a new, higher-numbered CANONICAL file that names this one as
  its source.

---

## 2. Purpose & Scope

### 2.1 Product summary

`spoken` is a C++17 command-line tool and reusable library that converts a line of
mixed text (words, numbers, symbols, punctuation) into a natural spoken-English
sentence, and optionally renders that sentence to audio (playback or WAV) via an
offline, open-source text-to-speech (TTS) engine.

### 2.2 Goals

- G1 — Pedagogical: exercise every load-bearing Tier 4 and Tier 5 concept from
  `000_cpp17-reference.md` in one coherent, real application.
- G2 — Correct: produce deterministic, testable text output for the grammar in
  Section 7.
- G3 — Portable: build and pass all tests on Linux, macOS, and Windows with
  GCC >= 9, Clang >= 7, or MSVC >= 19.15, under `-std=c++17` strictly.
- G4 — Degradable: build and run with no TTS library installed (null engine). Note
  (per review S-1): "builds without TTS" and "has full test coverage" are distinct
  guarantees; both hold, and both are stated separately (G4 here, NFR-3 below).

### 2.3 Non-goals

- Not a general NLP/grammar engine; the input grammar is the closed set in Section 7.
- Not a real-time/streaming audio mixer (extension exercise).
- Not internationalized in v1 (English only; locale is an extension).

### 2.4 Definitions

- Token: one lexical unit produced by the tokenizer (Section 6.1).
- Record: one input line/file processed into one spoken sentence.
- Engine: a `TtsEngine` implementation (real `espeak` or `null`).
- Null engine: a `TtsEngine` that emits spoken text to a sink instead of audio.

---

## 3. Requirements

### 3.1 Functional requirements (FR)

- FR-1 (MUST): tokenize input into Word / Integer / Decimal / Symbol /
  Punctuation / Whitespace tokens.
- FR-2 (MUST): convert integers (including negatives, up to about +/- 9.2e18) to words.
- FR-3 (MUST): convert decimals to English ("3.14" -> "three point one four").
- FR-4 (MUST): convert known symbols via a lookup table; spell unknown symbols by name.
- FR-5 (MUST): assemble per-token spoken forms into one capitalized, terminally
  punctuated sentence with correct spacing (no space before `.,!?;:`).
- FR-6 (MUST): read input from positional args, `--file`, or stdin.
- FR-7 (MUST): write spoken text to stdout or `--out <path>`.
- FR-8 (SHOULD): speak aloud via the TTS engine unless `--no-speak`/`--null-tts`.
- FR-9 (SHOULD): synthesize WAV via `--wav <path>`.
- FR-10 (SHOULD): batch-process every `*.txt` in `--batch <dir>` concurrently, with
  deterministic ordering of the report.
- FR-11 (MUST): select engine via factory (Auto/Espeak/Null); fall back to null when
  eSpeak is unavailable.
- FR-12 (MUST): expose the conversion pipeline and factory as a linkable library API.
- FR-13 (MUST): report errors via a typed exception hierarchy; the CLI maps them to
  exit codes.
- Note: currency fusion ("$99.99" -> "ninety-nine dollars and ninety-nine cents") is
  explicitly OUT of v1 scope (decision Q-2). It is Extension Exercise #2.

### 3.2 Non-functional requirements (NFR)

Promoted to their own section per review suggestion S1 so rule references resolve to
one place.

- NFR-1: C++17 only. No C++20 features (`starts_with`, designated initializers,
  `consteval`, ranges, `std::span`, `constinit`) and no compiler extensions.
- NFR-2: builds clean under `-Wall -Wextra -pedantic -Wshadow -Wconversion`
  (and `/W4 /permissive-` on MSVC).
- NFR-3: all unit and integration tests pass under ASan+UBSan; concurrency tests pass
  under TSan. Test coverage holds whether or not eSpeak is installed.
- NFR-4: the library has zero global mutable state; converters are value types
  (Rule of 0).
- NFR-5: pure conversion functions are deterministic and locale-independent ("C").
- NFR-6: public headers must not leak third-party C headers (eSpeak hidden by Pimpl).
- NFR-7: CLI exit codes per Section 8.3.
- NFR-8: build requires CMake >= 3.16 (target-based `pkg_check_modules` plus
  `target_sources(... PRIVATE)`).

---

## 4. Architecture

### 4.1 Layered component view

```
Presentation:  apps/spoken-cli   examples/batch_convert
                    | depends on interfaces only
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

Dependency rule: arrows point downward only. No layer reaches up. Presentation depends
on Domain via the `TtsEngine` interface (Dependency Inversion).

### 4.2 Data flow (single record)

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

## 5. Module Specifications & Interface Contracts

Each module lists Responsibility, API, Contract, and Errors.

- 5.1 `version.hpp`: compile-time `inline constexpr` version constants; header-only;
  `inline namespace v1` permitted (C++11).
- 5.2 `error.hpp`: `SpokenError : std::runtime_error` (root); `InvalidInputError`;
  `TtsError`; inheriting constructors. All library throws derive from `SpokenError`.
- 5.3 `tokens.hpp/.cpp`: define `Token` sum type and `operator<<`; exactly one active
  alternative; streaming is total via `if constexpr`. Integer/Decimal carry a
  `lexeme` field (decision Q-4) for exact decimal rendering.
- 5.4 `tokenizer.hpp/.cpp`: `[[nodiscard]] std::vector<Token> tokenize(string_view)`
  plus a free function; stateless; total; throws `InvalidInputError` only on a
  malformed numeric span.
- 5.5 `number_converter.hpp/.cpp`: `[[nodiscard]] std::string to_words(int64_t)` and
  `to_words(double)`; pure; output rules per Section 7.1-7.2; not `constexpr` in v1.
- 5.6 `symbol_converter.hpp/.cpp`: table-driven; `to_words(string_view)`,
  `known(string_view) noexcept`; Rule of 0.
- 5.7 `sentence_builder.hpp/.cpp`: `[[nodiscard]] std::string assemble(...)`;
  implements the Section 7.4 spacing rules exactly.
- 5.8 `conversion_pipeline.hpp/.cpp`: composition of the four converters (Rule of 0);
  thread-safe for concurrent `const` calls.
- 5.9 `tts/tts_engine.hpp`: abstract interface; virtual destructor; `speak`, `to_wav`,
  `[[nodiscard]] name()`. Contract: implementations may throw `TtsError` (this design
  rule now lives in the interface section, per review S-4).
- 5.10 `tts/espeak_engine.*`: `final`, move-only (move `noexcept`), Pimpl; header must
  not include eSpeak headers; compiled only when `SPOKEN_HAVE_ESPEAK`; WAV via the
  eSpeak synthesis API with a POSIX CLI fallback (decision Q-3).
- 5.11 `tts/null_engine.*`: `using Sink = std::function<void(string_view)>;` throws
  `TtsError` on empty sink; `to_wav` writes a placeholder file.
- 5.12 `tts/factory.hpp/.cpp`: `enum class EngineKind { Auto, Espeak, Null };`
  `struct EngineConfig {...};` `make_engine(const EngineConfig&)`. Auto resolves to
  Espeak iff available else Null. `EngineConfig` is constructed field-by-field (no
  designated initializers). `src/tts/factory.cpp` is a required source file in the
  tree and the build (resolves DEF-9).

---

## 6. Data Model

### 6.1 Token alternatives

- `Word`        : `std::string text`      — letters, `'`, `-`.
- `Integer`     : `std::int64_t value`, `std::string lexeme` — separators stripped
  from `value`; `lexeme` keeps the original digits (decision Q-4).
- `Decimal`     : `double value`, `std::string lexeme` — `lexeme` drives exact
  fractional rendering (decision Q-4).
- `Symbol`      : `std::string glyph`     — 1-2 char operators/symbols.
- `Punctuation` : `char mark`             — one of `.!,?;:`.
- `Whitespace`  : `std::string spaces`    — produced by the tokenizer but DROPPED by
  the pipeline before assembly (decision Q-1); the builder owns spacing.

`using Token = std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;`

### 6.2 Configuration types

- `EngineConfig { EngineKind kind=Auto; std::string voice="en";`
  `int speed_wpm=175; std::string text_sink; }`.
- `Args` (CLI) — see Section 8.

---

## 7. Behavioural Specification (Normative)

Tests assert against this section.

### 7.1 Integers (FR-2)

- `0` -> "zero"; `21` -> "twenty-one"; `100` -> "one hundred";
  `115` -> "one hundred fifteen".
- Tens-and-ones hyphenated; hundreds not followed by "and" (US style).
- Negatives prefixed "negative " and the prefix MUST be preserved (fixes DEF-3).
- Scale words: thousand, million, billion, trillion, quadrillion, quintillion.

### 7.2 Decimals (FR-3)

- `3.14` -> "three point one four"; `0.5` -> "zero point five";
  `-2.71` -> "negative two point seven one".
- Integer part via Section 7.1; then " point"; then each fractional digit of the
  ORIGINAL lexeme spoken individually — no trailing-zero padding, no spurious leading
  zero (fixes DEF-4 by reading `Decimal::lexeme`, not a reformatted double).

### 7.3 Symbols (FR-4)

- Known (normative subset): `$`->dollars, `%`->percent, `@`->at, `&`->and, `+`->plus,
  `-`->minus, `*`->times, `/`->divided by, `=`->equals, `!=`->not equals,
  `->`->arrow, `<`->less than, `>`->greater than.
- Unknown single chars spelled: `#`->hash, `~`->tilde, backtick->backtick,
  `^`->caret, `|`->pipe, backslash->backslash.

### 7.4 Sentence assembly (FR-5) — normative spacing rules

Given the per-token spoken forms (whitespace tokens already dropped, per Q-1),
`assemble` must:

1. Drop empty forms.
2. Join word/number/symbol forms with exactly one space.
3. Attach punctuation with no preceding space: "apples" + "." -> "apples."
   (fixes DEF-2).
4. Collapse internal whitespace runs to a single space; trim the ends.
5. Capitalize the first alphabetic character.
6. Ensure terminal punctuation: if the last non-space char is not one of `.?!`,
   append `.`.

### 7.5 Currency — out of v1 scope (decision Q-2)

The v1 grammar renders `$99.99` as `Symbol($) Decimal(99.99)` ->
"dollars ninety-nine point nine nine". Currency fusion is NOT implemented; the
`pipeline_currency` test is removed from the suite and recorded as Extension Exercise
#2 (resolves DEF-5). Acceptance tests use grammar-accurate expectations only.

---

## 8. CLI Specification

### 8.1 Synopsis

```
spoken [options] [TEXT...]
spoken --file <path> [options]
echo "..." | spoken [options]
spoken --batch <dir> [options]
```

### 8.2 Options

- `-h, --help` ; `-f, --file <path>` ; `-o, --out <path>` ; `-w, --wav <path>` ;
  `--no-speak` ; `--voice <name>` (default `en`) ; `--speed <wpm>` (default 175) ;
  `-j, --jobs <N>` (default hardware concurrency) ; `--batch <dir>` ; `--null-tts` ;
  `--verbose`.

### 8.3 Exit codes

Adds code 4 (the `catch (...)` path), per review S4.

- 0 — success.
- 1 — runtime / IO error.
- 2 — bad arguments.
- 3 — TTS init failure.
- 4 — unexpected / internal error (contract violation; `catch (...)` returns 4).

### 8.4 Argument parser contract

- Returns `std::optional<Args>`; `std::nullopt` causes exit 2.
- Must not use `std::string_view::starts_with` (C++20). Use `arg.rfind('-', 0) == 0`
  or `!arg.empty() && arg[0] == '-'` (fixes DEF-1).
- The top-level `main` must have a `catch (...)` returning 4 so a stray non-standard
  throw cannot terminate the process uncaught.

---

## 9. Concurrency Model

- Unit of parallelism: one record (file) per `std::async(std::launch::async, ...)`.
- `ConversionPipeline` is used `const` across threads; safe (NFR-4).
- `TtsEngine` is NOT thread-safe for `speak`/`to_wav`. In batch mode, conversion and
  file IO run in parallel, but engine calls MUST be serialized: either a `std::mutex`
  guards `speak`/`to_wav`, or one engine is built per task, or TTS runs after the
  parallel join. The sketch's shared-engine concurrent `to_wav` is a data race and is
  forbidden (fixes DEF-10).
- Progress counter: `std::atomic<std::size_t>` (relaxed is sufficient).
- Ordering guarantee (per review S7): input files are sorted by canonical path
  (lexicographic on `fs::path::string()`) before dispatch. The progress report
  ("[3/17]") follows the sorted order, not `directory_iterator` order (which is
  implementation-defined and varies across ext4/NTFS/APFS).
- Parallel STL: `std::execution::par` is OPTIONAL (decision Q-5). The default build
  uses sequential `std::sort`; parallel policy is enabled only where a backend (e.g.
  TBB) is available. TSan must be clean on the batch path (NFR-3).

---

## 10. Build, Configuration & Portability

### 10.1 Toolchain matrix

- Linux   : GCC >= 9 / Clang >= 7 ; TTS via apt `libespeak-ng-dev` ; primary.
- macOS   : AppleClang / Clang     ; TTS via brew `espeak-ng`       ; supported.
- Windows : MSVC >= 19.15          ; TTS via vcpkg `espeak-ng`      ; supported
  (the no-TTS path must work).

### 10.2 CMake contract

- `cmake_minimum_required(VERSION 3.16)`; `CMAKE_CXX_STANDARD 17`;
  `STANDARD_REQUIRED ON`; `EXTENSIONS OFF`.
- Options: `SPOKEN_BUILD_TESTS`, `SPOKEN_BUILD_EXAMPLES`,
  `SPOKEN_WARNINGS_AS_ERRORS`, `SPOKEN_WITH_ESPEAK`, `SPOKEN_SANITIZERS`.
- `SPOKEN_SANITIZERS` is a comma-separated list parsed into flags:
  `address`/`undefined` -> `-fsanitize=address,undefined -fno-omit-frame-pointer`;
  `thread` -> `-fsanitize=thread`; `memory` -> `-fsanitize=memory`. ASan and TSan are
  mutually exclusive (separate builds).
- Targets: library `spoken`; CLI `spoken-cli`; tests `spoken_tests`; example
  `batch_convert`. eSpeak detected via `pkg_check_modules`; defines
  `SPOKEN_HAVE_ESPEAK=0/1`; `espeak_engine.cpp` compiled only when found.

### 10.3 Portability requirements (resolve sketch gaps)

- `espeak_engine.cpp`: no `mkstemp`/`::close`/`/tmp`/`std::system(... 2>&1)` on the
  default path. Use `std::filesystem::temp_directory_path()`, gate any POSIX-only code
  behind `#ifdef`, and prefer the eSpeak synthesis API for WAV (fixes DEF-6). Add the
  required includes (`<fstream>`, `<cstdio>`, and POSIX headers only inside the gate).
- `factory.cpp`: cross-platform availability detection (or rely on the CMake
  `SPOKEN_HAVE_ESPEAK` define); include `<iostream>` for the Null branch (fixes DEF-7).

### 10.4 Sanitizers & CI

- ASan+UBSan build green for unit/integration tests; a separate TSan build green for
  the batch path. CI (GitHub Actions) builds both sanitizer configs across the
  toolchain matrix and runs `ctest`.

---

## 11. Test Plan & Acceptance Criteria

### 11.1 Levels

- Unit: tokenizer, number converter, symbol converter, sentence builder.
- Integration: `ConversionPipeline` end-to-end.
- System (CI): CLI single + batch, exit codes (incl. 4), WAV/null behavior.
- Sanitizer: unit/integration under ASan+UBSan; batch under TSan.

### 11.2 Requirement-to-test traceability (literal test names, per review S8)

- FR-1  -> tokenize_words, tokenize_numbers, tokenize_symbols_vs_punctuation,
  tokenize_multi_char_operator.
- FR-2  -> small_integers, negatives, thousands_and_millions (DEF-3 must be fixed).
- FR-3  -> decimals (DEF-4 must be fixed).
- FR-4  -> symbol_lookup, symbol_fallback.
- FR-5  -> capitalize_and_period, collapse_whitespace,
  preserve_terminal_punctuation (DEF-2 must be fixed).
- FR-10 -> system batch test + TSan (DEF-10 must be fixed).
- FR-11 -> factory test (Auto/Null/Espeak) — to be added.
- FR-13 -> CLI exit-code tests incl. code 4 — to be added.
- Removed: pipeline_currency (descoped per Q-2).

### 11.3 Acceptance criteria (Definition of Done)

1. `cmake --build` succeeds on all three platforms with no warnings (NFR-2).
2. `ctest` green, including the corrected expectations from Section 7.
3. ASan+UBSan and TSan builds green in CI.
4. No C++20 constructs anywhere (Section 14 audit all OK).
5. Public headers compile standalone and leak no third-party headers (NFR-6).
6. All Section 12 defects Resolved or explicitly Accepted with a note.

---

## 12. Defect Register

### 12.1 Summary (structural edit A)

```
DEFECT REGISTER — SUMMARY
=========================
Total entries:     13
Blocking sign-off:  9 (DEF-1, 2, 3, 4, 6, 7, 8, 10, 11)
Doc/cosmetic:       3 (DEF-9, 12, 13)
Decision-resolved:  1 (DEF-5 -> descoped per Q-2)
```

### 12.2 Status

|--------|-----|----------|
| ID     | Sev | Status   |
|--------|-----|----------|
| DEF-1  | S1  | Planned  |
| DEF-2  | S2  | Planned  |
| DEF-3  | S2  | Planned  |
| DEF-4  | S2  | Planned  |
| DEF-5  | S2  | Resolved |
| DEF-6  | S3  | Planned  |
| DEF-7  | S3  | Planned  |
| DEF-8  | S1  | Planned  |
| DEF-9  | S4  | Doc fix  |
| DEF-10 | S3  | Planned  |
| DEF-11 | S1  | Planned  |
| DEF-12 | S4  | Doc fix  |
| DEF-13 | S4  | Teaching |
|--------|-----|----------|

### 12.3 Where each defect is fixed (structural edit B)

- DEF-1  -> apps/spoken.cpp parse_args: replace `starts_with` (Section 8.4).
- DEF-2  -> src/sentence_builder.cpp + drop Whitespace in pipeline (Sections 6.1, 7.4).
- DEF-3  -> src/number_converter.cpp: build magnitude separately (Section 7.1).
- DEF-4  -> tokens.hpp/.cpp add `lexeme`; number_converter reads it (Sections 6.1, 7.2).
- DEF-5  -> descoped to Extension Exercise #2; remove pipeline_currency (Section 7.5).
- DEF-6  -> src/tts/espeak_engine.cpp: temp_directory_path + #ifdef + includes (10.3).
- DEF-7  -> src/tts/factory.cpp: cross-platform detect + `<iostream>` (Section 10.3).
- DEF-8  -> factory call sites: construct EngineConfig field-by-field (Section 5.12).
- DEF-9  -> add src/tts/factory.cpp to tree + concept map (Section 5.12).
- DEF-10 -> apps/spoken.cpp batch: serialize engine or one per task (Section 9).
- DEF-11 -> tests: runtime check, not static_assert on a non-constexpr string (11.x).
- DEF-12 -> correct the "GCC 8 / inline namespace" troubleshooting note.
- DEF-13 -> keep batch_convert.cpp bug as labeled teaching; exclude from CI gates.

### 12.4 Accepted trade-offs (review suggestion S3)

- AT-1: eSpeak may shell out via the `espeak-ng` CLI on POSIX as a fallback.
  Rationale: pedagogy over production; the Pimpl still demonstrates the idiom. Default
  path uses the synthesis API (Q-3).
- AT-2: `examples/batch_convert.cpp` ships with a deliberate capture-by-ref bug.
  Rationale: documented as DEF-13; excluded from CI correctness gates.
- AT-3: `ConversionPipeline` is cheap to construct per-thread rather than shared.
  Rationale: avoids shared-state hazards; cost is negligible (Rule of 0 value type).

---

## 13. Decisions Log (Open Questions Resolved)

All five open questions are resolved to their recommended defaults.

- Q-1 (spacing model): RESOLVED — drop `Whitespace` tokens in the pipeline; the
  `SentenceBuilder` owns all spacing. Affects DEF-2.
- Q-2 (currency scope): RESOLVED — currency fusion is OUT of v1; descope the test and
  record as Extension Exercise #2. Resolves DEF-5.
- Q-3 (WAV strategy): RESOLVED — use the eSpeak synthesis API on the default path;
  CLI fallback only inside a POSIX `#ifdef`.
- Q-4 (decimal source): RESOLVED — add a `lexeme` field to `Integer`/`Decimal`;
  decimals render from the lexeme. Affects DEF-4.
- Q-5 (parallel policy): RESOLVED — parallel STL is optional with a sequential
  fallback by default.

---

## 14. C++17 Conformance Audit (review S6)

- OK : `if constexpr` in template dispatch.
- OK : structured bindings (Args, tests).
- OK : `std::filesystem` (CLI paths).
- OK : `std::optional` (parse_args, file helpers).
- OK : `std::variant` + `std::visit` (Token).
- OK : fold expressions (test framework).
- OK : inline variables / inline namespace.
- OK : `<execution>` parallel STL (optional path).
- NO : `std::string_view::starts_with` -> DEF-1.
- NO : designated initializers          -> DEF-8.
- NO : `constexpr std::string`          -> DEF-11.

---

## 15. Implementation Sequence (review S10)

```
Phase 1 (compile green):   DEF-1, DEF-8, DEF-11  + add missing #includes
Phase 2 (output correct):  DEF-2, DEF-3, DEF-4   + update test expectations
Phase 3 (portability):     DEF-6, DEF-7          + Windows CI matrix
Phase 4 (concurrency):     DEF-10                + TSan CI job
Phase 5 (decisions baked): Q-1, Q-2, Q-4 already decided; verify in code
Phase 6 (polish):          DEF-9, DEF-12, DEF-13 + exit code 4 + NFR-8
```

The corrected implementation is delivered as a new file (e.g.
`008_spoken_corrected_impl.md`) per R-Naming-1, then the code is written from it.

---

## 16. Scoring Convention (closes review R1/R6)

To stop rating drift across the review chain:

- Format score ceiling is 10.0 by construction: rules-compliance is binary (a doc
  meets the line-length/no-tab/table rules or it does not).
- Content score cannot exceed `min(10.0, source_content_score + 1.0)` — a reformat may
  add clarity but not new substance.
- When a rubric mean is reported, it is the exact arithmetic mean, labeled as such
  (e.g. "Mean across 10 rubric criteria: 9.0 / 10"), not an approximation.
- Per-artifact ratings live in a single portfolio file, not re-printed in each doc,
  to avoid the table growing unboundedly (review I2).

---

## 17. Sign-off Checklist

- [ ] Section 3 requirements approved (FR + NFR).
- [ ] Section 7 normative rules approved as the test oracle.
- [x] Q-1..Q-5 decided (Section 13).
- [ ] DEF-1, DEF-8, DEF-11 (compile breaks) scheduled — Phase 1.
- [ ] DEF-2, DEF-3, DEF-4 (output bugs) scheduled with corrected tests — Phase 2.
- [ ] DEF-6, DEF-7, DEF-10 (portability/race) scheduled — Phases 3-4.
- [x] DEF-5 resolved (descoped, Section 7.5).
- [ ] CI matrix (Linux/macOS/Windows x {ASan+UBSan, TSan}) agreed.
- [ ] Acceptance criteria (Section 11.3) accepted as the Definition of Done.
- [ ] Project rules R-Rendition-1, R-Naming-1, R-Canonical-1 adopted (Section 1).

On sign-off, the next artifact is the corrected implementation (a new file per
R-Naming-1), and coding proceeds from it — not another rating rendition.
