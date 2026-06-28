# Rating: `spoken` Design & Specification

> Rules-compliant rendition of an external review/rating of the `spoken` design
> spec (`002_spoken_design_and_spec.md` and its compliant rendition
> `003_spoken_design_and_spec.md`). Reformatted to `_mydocs/002_rules.md`
> (lines <= 120 chars, no tabs, no GitHub-style pipe-bordered tables). Substance
> is preserved verbatim in meaning; only layout changed.

---

## Overall: 8.7 / 10 — A serious, reviewable spec that does what a spec should do

This is a substantial upgrade from a sketch to an artifact you can sign off on,
hand to a contributor, or audit yourself in six months. The defect register alone
is worth the rewrite — it converts "I think this works" into "here are the things
that don't, and here's what to do about them." That's the entire point of a spec.

Below is a section-by-section rating, then a small set of structural suggestions you
may want to apply before treating this as canonical.

---

## Section scores

- Section 1 — Purpose & Scope — 9/10: goals/non-goals/definitions are tight;
  minor: G4 conflates "no TTS" with "test coverage" — these are different things.
- Section 2 — Requirements — 8/10: FR/NFR split is good; NFR-2 should also mention
  `-Wshadow`/`-Wconversion` (you use them in the sketch).
- Section 3 — Architecture — 9/10: the layered diagram plus "arrows point down only"
  is the single most useful sentence in the doc.
- Section 4 — Module Specs — 9/10: contracts are clear; the design rule
  "implementations may throw `TtsError`" belongs in 4.9 itself.
- Section 5 — Data Model — 9/10: concrete and unambiguous; `Token` rules match the
  sketch exactly.
- Section 6 — Behavioural Spec — 10/10: this is the heart, and it's done well —
  especially 6.4's spacing rules and the cross-reference to the sketch's failure.
- Section 7 — CLI Spec — 8/10: exit-code table is correct and useful; see suggestion
  S4 below.
- Section 8 — Concurrency Model — 9/10: catching the shared-engine race (DEF-10) is
  exactly the kind of thing specs should do.
- Section 9 — Build & Portability — 8/10: missing minimum CMake version, Ninja
  recommendation, and the actual `SPOKEN_SANITIZERS` parsing rules.
- Section 10 — Test Plan — 9/10: the traceability matrix (FR -> test) is gold.
- Section 11 — Defect Register — 10/10: best part of the document. Severity + status
  + fix is a clean triaging format.
- Section 12 — C++17 Conformance — 9/10: good; add `if constexpr`, fold expressions,
  `std::filesystem`, `std::optional` to the "OK" column.
- Section 13 — Open Questions — 9/10: all five are real; see suggestion S9 below.
- Section 14 — Sign-off Checklist — 9/10: strong closing artifact; one tiny refactor
  would make it stronger.

---

## What's excellent

1. The defect register is exemplary. Severity + status + fix + line pointer is
   exactly how engineering specs should triage issues. Thirteen entries is a lot for
   a single document — but that's the point: a sketch hides these, a spec surfaces
   them.

2. The behavioural spec (Section 6) is normative. Stating rules as test oracles
   ("3.14 -> 'three point one four'") is what lets the tests be unambiguous. The
   DEF-5 cross-reference to the contradictory test is a model of how to flag spec
   contradictions instead of letting them ship.

3. The concurrency section makes a real claim. Section 8 doesn't just say "use
   threads" — it specifies one engine per task vs. shared engine with mutex, calls
   out TSan, and refuses to claim parallelism without measurement. That's
   professional-grade thinking.

4. The architecture diagram says "no, you can't reach up". Single sentence, enormous
   value. Most OOP violations start with someone reaching up a layer.

5. The data-race catch (DEF-10). Spotting that the comment in the sketch says
   "serialize" but the code doesn't is exactly the kind of inconsistency a spec
   review is supposed to catch.

---

## What's good but improvable

### S1 — NFRs need a section number, not bullets buried in 2.2

The non-functional requirements are valuable but live as a bullet list at the bottom
of Section 2. They're referenced from 4.10 (NFR-6), 8 (NFR-3, NFR-4), 9 (NFR-2).
Consider promoting NFRs to their own Section 2.3 or splitting them into a small
appendix:

```
Section 2.3 Non-functional Requirements (NFR)
   NFR-1  Strict C++17, no GNU/C++20 features
   NFR-2  Warning posture (-Wall -Wextra -pedantic -Wshadow -Wconversion)
   NFR-3  Sanitizer discipline (ASan+UBSan always; TSan on CI)
   NFR-4  No global mutable state (Rule of 0)
   NFR-5  Deterministic, locale-independent ("C")
   NFR-6  No third-party headers in public API
   NFR-7  CLI exit codes
```

Costs nothing. Makes the "where does this rule live?" question answerable.

### S2 — Section 6.5 has a subtle category error

Section 6.5 says "Currency fusion ... is NOT in v1 scope." But Section 10's
traceability matrix (FR-4 -> symbol table) and 10.2 (`pipeline_currency` test) both
treat currency as testable. Either:

- (a) remove the test and add a Section 11 extension exercise entry (you already have
  it as #2), or
- (b) promote currency fusion to FR-13a and add a Section 6.6 "Currency fusion"
  subsection with normative rules.

The current state — "it's not in scope, but we have a test for it" — is exactly the
ambiguity specs are supposed to eliminate. This is the most important fix to apply.

### S3 — The defect register should also list "non-defects intentionally accepted"

Right now DEF-13 is the only entry that says "we know about this and accept it."
There are probably others:

- the eSpeak-via-shell approach (flagged as acceptable for pedagogy);
- the use of `std::system` for batch on POSIX (defended with `#ifdef` per Section
  9.3, but is the `#ifdef` coverage itself tested?);
- the fact that `ConversionPipeline` is per-thread-constructible (cheap) vs. shared
  (free) — a design choice, not a defect, but not documented.

A short "Accepted Trade-offs" list parallel to the defect register would close the
loop. Example:

```
AT-1  eSpeak shells out via espeak-ng CLI on POSIX
      Rationale: pedagogy > production; Pimpl still demonstrates the idiom.
AT-2  batch_convert.cpp ships with a capture-by-ref bug
      Rationale: documented as DEF-13; excluded from CI.
```

### S4 — Section 7.3 has one missing exit code

Current codes: 0 success; 1 runtime / IO error; 2 bad arguments; 3 TTS init failure.

You're missing "internal / unexpected exception" (the `catch (...)` path). Add:

```
4  unexpected error (unreachable / contract violation)
```

And in the catch hierarchy in `apps/spoken.cpp`, make sure `catch (...)` returns 4.
The sketch's top-level catch only handles `SpokenError` and `std::exception`. A
`throw 42;` (yes, that's legal in C++) would terminate the process — fix or document.

### S5 — Section 9 should specify the CMake minimum version explicitly

"Linux: GCC >= 9" is fine, but the spec doesn't say
`cmake_minimum_required(VERSION 3.16)`. The sketch does. Pick the version, declare it
in Section 9.2, and add it to NFR:

> NFR-8: build requires CMake >= 3.16 (target-based `pkg_check_modules` plus
> `target_sources(... PRIVATE)` requires a modern-targets CMake; 3.16 makes the
> requirement explicit).

### S6 — The C++17 checklist (Section 12) is missing affirmative items

The "OK" column is mostly empty. Fill it in to make the checklist actively useful:

```
OK  if constexpr in template dispatch (sketch 7.21)
OK  structured bindings (sketch apps/spoken Args, tests)
OK  std::filesystem (sketch apps/spoken.cpp)
OK  std::optional (sketch apps/spoken.cpp parse_args)
OK  std::variant + std::visit (sketch 7.21)
OK  fold expressions in test framework
OK  inline variables (sketch 7.7)
OK  <execution> parallel STL (sketch apps/spoken.cpp batch)
NO  std::string_view::starts_with   -> DEF-1
NO  designated initializers          -> DEF-8
NO  constexpr std::string            -> DEF-11
```

Now it reads as an audit, not just a checklist.

### S7 — Section 8 should state the ordering guarantee explicitly

"the input file list is sorted ... for deterministic reporting (FR-10)" — sort by
what key? Specify:

```
Ordering: input files are sorted by canonical path (lexicographic on
fs::path::string()) before being dispatched to async tasks. The progress report
("[3/17]") follows the sorted order, not the directory enumeration order.
```

This matters because `fs::directory_iterator` order is implementation-defined and
differs across filesystems (ext4 vs. NTFS vs. APFS).

### S8 — Section 10's traceability could link to specific test names

You list "FR-2 -> test_number_converter (small/negatives/thousands)" but it's
hand-paraphrased. Replace with the literal test names from the sketch:

```
FR-2 -> small_integers, negatives, thousands_and_millions
FR-3 -> decimals
```

Costs nothing; saves the next reader a search.

### S9 — Section 13 has 5 questions; recommend answers inline as a draft

A spec with unanswered questions is honest, but it is stronger if it ships with a
recommended default for each Q that the reviewer can accept with one check-mark
rather than read from scratch. You have recommendations in parentheses; promote them
to "Default answer:" headers so the sign-off checklist can be:

```
[ ] Q-1 accepted: whitespace tokens dropped (recommended)
[ ] Q-2 accepted: descope currency fusion from v1 (recommended)
[ ] Q-3 accepted: eSpeak synthesis API; CLI fallback only on POSIX
[ ] Q-4 accepted: add lexeme field to Decimal/Integer tokens
[ ] Q-5 accepted: parallel STL optional, sequential fallback by default
```

### S10 — Section 14 should include a "post-sign-off plan"

After sign-off, what's the implementation order? Right now Section 14 is a punch-list
of approvals. Add a Section 14.1 "Implementation sequence" that turns sign-off into a
work plan:

```
Phase 1 (compile green):   DEF-1, DEF-8, DEF-11  + add missing #includes
Phase 2 (output correct):  DEF-2, DEF-3, DEF-4   + update test expectations
Phase 3 (portability):     DEF-6, DEF-7          + Windows CI matrix
Phase 4 (concurrency):     DEF-10                + TSan CI job
Phase 5 (decisions):       Q-1, Q-2, Q-4
Phase 6 (polish):          Section 9 CMake version, NFR-8, exit code 4
```

This is what turns a spec into a project plan.

---

## What I'd change in the document structure

Three structural edits would make this significantly more reviewable.

### A. Add a top-level defect summary box

```
DEFECT REGISTER — SUMMARY
=========================
Total entries:     13
Blocking sign-off:  9 (DEF-1, 2, 3, 4, 6, 7, 8, 10, 11)
Doc/cosmetic:       3 (DEF-9, 12, 13)
Decisions:          1 (DEF-5 -> Q-2)
```

### B. Add a "where each defect is fixed" mapping

After Section 11, add a small mapping:

```
DEF-1   -> apps/spoken.cpp parse_args: replace starts_with
DEF-2   -> src/sentence_builder.cpp + drop Whitespace tokens in pipeline
DEF-3   -> src/number_converter.cpp: build magnitude separately
DEF-4   -> tokens.hpp/cpp: add lexeme field to Integer/Decimal
DEF-6   -> src/tts/espeak_engine.cpp: temp_directory_path + #ifdef + includes
DEF-7   -> src/tts/factory.cpp: cross-platform detection + <iostream>
DEF-8   -> factory call sites: drop designated initializers
DEF-10  -> apps/spoken.cpp batch: serialize engine or one engine per task
DEF-11  -> tests/test_number_converter.cpp: runtime check, not static_assert
```

This is the single thing that turns "I know what's wrong" into "I know where the fix
goes".

### C. Add an "open decisions" status box at the top

```
OPEN DECISIONS BLOCKING SIGN-OFF
================================
Q-1 (spacing model):   [PENDING]
Q-2 (currency scope):  [PENDING]  <- blocks DEF-5
Q-4 (token lexeme):    [PENDING]  <- blocks DEF-4 fix
```

---

## Rating detail by rubric

If you're scoring against a rubric (e.g. for a code review or class assignment):

- Completeness — 9/10: all required spec sections present; missing post-sign-off
  plan and an accepted-trade-offs section.
- Correctness — 9/10: catches real defects in the sketch; DEF-5 contradiction
  surfaced honestly.
- Internal consistency — 9/10: one contradiction (6.5 vs 10.2) noted as itself
  needing a decision — that's the right move.
- Testability — 10/10: Section 10 traceability matrix + Section 6 normative rules
  means tests can be written from the spec alone.
- Traceability (requirements -> test -> defect) — 9/10: add the "where each defect is
  fixed" mapping (suggestion B) and this is 10/10.
- Portability awareness — 8/10: catches POSIX-only code (DEF-6/7); doesn't yet
  specify Windows behavior for `--batch` or WAV.
- C++17 discipline — 9/10: catches the two real C++20 leaks; minor: doesn't grep the
  sketch for other accidental C++20 (e.g. `constinit`, `std::span`).
- Concurrency rigor — 10/10: DEF-10 is exactly the right thing to flag.
- Pedagogical value — 9/10: spec doubles as a learning artifact; the tier-by-tier
  mapping in the original sketch still applies.
- Editorial polish — 8/10: ASCII tables are clean; borders work; minor: 11.1 status
  could include "Verified" for closed defects.

Weighted average: ~8.9 / 10.

---

## What I'd actually do next

In priority order, working from highest leverage:

1. Apply suggestions S1 + S2 + B above. They take 15 minutes and convert the doc from
   "good spec" to "great spec" by closing the contradictions and adding the
   defect-fix mapping.
2. Decide Q-1, Q-2, Q-4 explicitly. Right now those three are blocking the
   implementation plan. Pick the recommended defaults, mark them ACCEPTED, and the
   rest of Section 14 becomes a straightforward punch list.
3. Apply the implementation sequence (suggestion S10). Once Q-1/Q-2/Q-4 are decided,
   the work plan is mechanical. That's when you can stop reviewing the spec and start
   writing the corrected code.
4. Optional polish: suggestions S3 (accepted trade-offs), S4 (exit code 4), S7
   (ordering guarantee), S9 (default-answer headers) — all valuable, none blocking.

---

## Final word

The biggest compliment for this spec: it's actually reviewable. Most design docs fail
at "can a third party audit this without talking to the author?" The defect register,
the normative Section 6 rules, and the FR-traceability matrix answer that with yes.

The biggest fix to push for: resolve Q-2 (currency scope) and Q-4 (token lexeme)
before signing off. Those two decisions unblock the implementation phases; everything
else is sequencing.

When the fixes from Section 11 are applied, drop the corrected artifact as a new file
(per the project rules) and request a second-pass review specifically against the
corrected code rather than the original sketch. That's where the next round of value
lives.
