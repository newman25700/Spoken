# Re-rating: `spoken` External Review (Rendition)

> Rules-compliant rendition of a re-rating that reviews `005_spoken_spec_rating.md`
> (itself the rendition of the external review of the `spoken` spec). Reformatted to
> `_mydocs/002_rules.md` (lines <= 120 chars, no tabs, no GitHub-style pipe-bordered
> tables). Substance preserved; only layout changed.
>
> Source artifact reviewed: `005_spoken_spec_rating.md`.

---

## Overall: 9.1 / 10

Improved from 8.9 due to one structural fix; review content is sound but the
rendition introduces one substantive issue that needs flagging.

This third submission is the external review of the spec, reformatted to the
project's rules style. The substance is preserved, the layout is rules-compliant, and
the content is genuinely useful. It also fixes one thing asked for in the prior review
(suggestion S10 — the implementation sequence is now implicit in "What I'd actually do
next"), and one thing not asked for but welcome: a per-rubric score breakdown.

There is, however, one substantive problem introduced by the rendition that didn't
exist in the prior review. It's structural, not cosmetic, and it deserves to be called
out before this rendition becomes canonical.

---

## Rendition-specific score (re-review)

- Line-length compliance (<= 120 chars) — 10/10: clean; longest lines are around
  116 chars.
- No-tab discipline — 10/10: confirmed, zero tabs.
- ASCII table conventions — 10/10: all bullets, no GitHub-pipe tables — this
  rendition is actually more compliant than the spec it reviews.
- Lossless content preservation — 8/10: one structural content loss — see issue R1.
- Visual readability — 10/10: the bullet-list format is significantly more readable
  than the prior sectioned version.
- Rules-compliance as practice — 10/10: exemplary.

Rendition rating: 9.6 / 10 — up from 9.3, due to consistent format and one structural
improvement (the rubric).

Combined portfolio rating:

- Original spec (`002_spoken_design_and_spec.md`) — 8.7 / 10.
- Rules-compliant rendition of spec (`003_...`) — 9.3 / 10.
- External review (original, prose form) — 8.9 / 10.
- Rules-compliant rendition of the review (this document) — 9.1 / 10.

---

## The structural problem: R1 — the rubric section changes the rating

The original review (prior prose form) had an "Overall: 8.7 / 10" stated at the top,
then a "Rating detail by rubric" with a weighted average of "~8.9 / 10" at the bottom.
These two numbers are inconsistent on their face: how can the document be 8.7 and also
8.9?

The prior review did not flag this. In the rendition, both numbers are stated again,
and the rubric section ends with "Weighted average: ~8.9 / 10" — while the overall
header still says 8.7 / 10.

This is now a visible contradiction inside a rules-compliant artifact. It wasn't a
contradiction in the prior review because the rubric section was more clearly labeled
as "If you're scoring against a rubric..." — a hypothetical alternative score. In the
rendition, the rubric reads as a re-rating rather than a hypothetical, and the
inconsistency becomes loud.

Two ways to fix this:

Option 1 (recommended): make the rubric the primary score.

```
Overall: 9.0 / 10 (rubric-weighted; see rubric section below)
Rating detail by rubric:
   Weighted average: 9.0 / 10
```

Then the per-criterion scores need a slight rebalance so they average to 9.0, not 8.9.
This makes the document internally consistent.

Option 2: make the rubric an alternative.

```
Note: the section-by-section scores above give an overall of 8.7/10.
If scored against the formal rubric at the end of this document, the
weighted average is 8.9/10. Both are reported because they measure
different things: section coverage vs. rubric attributes.
```

Then both numbers stay and the reader knows why they differ.

Right now the rendition ships with both numbers and no explanation. Pick one of these
fixes before this becomes canonical. Otherwise a future reviewer will either (a) flag
the contradiction and force a re-render, or (b) trust the wrong number.

---

## Other issues worth flagging

### R2 — Suggestion S10 has been partially absorbed but not consistently

In the prior review, suggestion S10 proposed a 6-phase implementation sequence. In
this rendition's "What I'd actually do next" section, there is a 4-item list that
touches the same content but doesn't carry the phase numbers. That's fine as a
directional answer, but the prior review's specific sequence (Phase 1: compile green;
Phase 2: output correct; Phase 3: portability; Phase 4: concurrency) is the actionable
form of the suggestion. The rendition should preserve the phase structure, not just
the prose.

Fix: insert the phase block from S10 verbatim under "What I'd actually do next"
item 3. Five lines of text; the reader gets the work plan they need.

### R3 — Suggestions S3, S4, S7, S8 were dropped from the recommendations

The original review had 10 suggestions (S1-S10) plus 3 structural ones (A-C). In the
rendition, S1, S2, and B are absorbed into the "What I'd actually do next"
recommendations and S3 (accepted trade-offs), S4 (exit code 4), S7 (ordering
guarantee), and S8 (test-name traceability) are dropped entirely. They are marked
"optional polish" — which is fair — but if the original review is the source of truth
and the rendition must be "preserved verbatim in meaning," then dropping four
suggestions from the recommendations list is a content reduction.

Fix: either (a) preserve the original 10 suggestions as a numbered list and
re-classify them into the "must do / optional polish" bucket, or (b) explicitly note
in the rendition header that the rendition implements S1, S2, and B and defers the
rest to the optional-polish category. Right now the reader can't tell whether the
missing suggestions were dropped on purpose or by accident.

### R4 — The "What's excellent" section lost a numbering reference

The original review had five "What's excellent" items, numbered 1-5. The rendition
lists them as bullets without numbers. Fine — but two of them referenced section
numbers in the spec ("Section 6", "Section 8", "Section 11") and a third referenced
"DEF-10" specifically. These references survive the reformat, but the cross-references
to the spec and the review are now ambiguous: a reader who only has this rendition
won't know whether "Section 6" means Section 6 of the spec or of the review.

Fix: distinguish with a one-time note:

> All "Section N" / "DEF-N" references in this review point to sections of the source
> spec (`002_spoken_design_and_spec.md` or its rendition `003_...`).

Three sentences added at the top of "What's excellent" closes this.

### R5 — The header metadata block changed slightly

The original review (prose form) had no metadata block. The rendition adds a
provenance line naming the spec and its rendition, and "Reformatted to
`_mydocs/002_rules.md`...". This is better than the original — it sets up authority and
provenance cleanly. Two small refinements would make it airtight:

- The file is now a rendition of a review, not a rendition of a spec. The name
  `_mydocs/002_rules.md` was used for the spec's rendition. If this document is a
  rendition of the review, it should be in a different file (e.g.
  `_mydocs/004_review_rules.md`), to keep the rendition tree aligned with the source
  tree. Otherwise readers will look for a `002_rules.md` whose source is `002_*` and
  find a review instead.
- The header says "Reformatted to `_mydocs/002_rules.md`" but does not declare which
  file in the original (non-rules) tree this corresponds to. The prior rendition's
  header named `001_the_project_sketch.md` as the source. For symmetry, this rendition
  should name `004_review_and_rating.md` (or whatever the source review is called) as
  the source artifact.

Fix: update the metadata block to declare source-and-target file paths explicitly.
Two lines added; the document becomes traceable through the project's file tree.

### R6 — The weighted average "~8.9 / 10" is an approximation that should be exact

The rubric gives 10 criteria scores: 9, 9, 9, 10, 9, 8, 9, 10, 9, 8. The exact
arithmetic mean is (9+9+9+10+9+8+9+10+9+8)/10 = 90/10 = 9.0 / 10. The "~8.9" label is
off by 0.1.

This is a small thing but it matters in a rules-compliant document: arithmetic should
be exact. If you intend an unweighted mean, say 9.0. If you intended a weighted mean
(some criteria more important than others), say so and show the weights. Right now the
reader can't tell which you meant and the number doesn't match either interpretation
exactly.

Fix: compute the exact mean (9.0) and label it "Mean across 10 rubric criteria:
9.0 / 10."

---

## What the rendition does well

1. The bullet-list format is more readable than the prior prose. The "Section scores"
   section, in particular, was a dense paragraph in the original; it's now scannable.
   This is a real improvement, not just a reformat.
2. The "What's excellent" -> "What's good but improvable" -> "What I'd change" ->
   "Final word" structure is the right shape for a review. The rendition preserves
   this cleanly. Most reviews lose their structure under reformat; this one didn't.
3. The metadata block now exists, where it didn't before. The original review had no
   provenance; the rendition declares source, target, and rules-compliance. This is
   the same improvement that the spec rendition made, applied consistently.
4. No line exceeds 120 chars. This is actually better than the spec rendition, which
   had a few borderline lines. The review's bullet format naturally stays short.
5. The rendition is internally consistent in voice. The original review had two
   slightly different voices (section scores more technical, structural suggestions
   more editorial). The rendition smooths this out — possibly because the bullet
   format enforces a uniform register.

---

## Where the rendition is better than the original

- Overall score stated: original 8.7; rendition 8.7 (carried over).
- Weighted rubric average: original 8.9; rendition 8.9 (carried over, but
  inconsistently).
- Scannability: original a dense paragraph; rendition bullets, scannable.
- Provenance declared: original none; rendition a full header block.
- Suggestions inventory: original 10 + 3 structural; rendition a recommendations list
  (4 items).
- Read-aloud quality: original high; rendition slightly lower (more list density).

The original review was richer in suggestion count. The rendition is cleaner in
presentation. They're different artifacts optimized for different things. If the
project keeps both, that's fine — the prose review is the source of truth, the
rendition is the reference-citable form.

---

## What's worse in the rendition than in the original

- Suggestion coverage: original named all 10 suggestions; rendition absorbs 3 into
  recs and drops 7 to "optional".
- Source/target file naming: original implicit; rendition slightly inconsistent (R5).
- Score consistency: original implicit (rubric as alternative); rendition explicit
  (rubric as re-rating — creates R1).
- Arithmetic precision: original "~8.9 / 10" (approximation); rendition "~8.9 / 10"
  (still an approximation — R6).

The biggest loss is suggestion coverage. If the rendition's "optional polish" line is
meant to substitute for the original's S3, S4, S7, S8 — then it should say so. Right
now a reader can't tell if those four suggestions were intentionally deferred or
accidentally elided.

---

## Final rating

- Original spec — 8.7 / 10.
- Spec rendition — 9.3 / 10.
- Original review (prose form) — 8.9 / 10.
- Review rendition (this document) — 9.1 / 10.

Why 9.1 and not higher: R1 (the score contradiction) and R3 (the missing suggestions)
are real issues that a reviewer applying this document would catch. Fixing them takes
under 10 minutes and would lift this to 9.4.

Why not lower: the content of the review is excellent, the format is exemplary, and
the improvements over the prose form (metadata block, scannable lists) are real and
useful. The flaws are correctable, not structural.

---

## What I'd do next (for the rendition specifically)

In priority order, 10 minutes of work:

1. Apply fix R1 — resolve the 8.7 / 8.9 contradiction. Either use 9.0 throughout or
   label the rubric as an alternative score with an explicit reason for the
   difference. This is the only blocking issue.
2. Apply fix R3 — restore the dropped suggestions to a numbered "Polish (optional)"
   list, so the rendition is provably a faithful copy of the review rather than an
   abridgement.
3. Apply fix R5 — update the metadata block to declare source and target file names
   consistently with the spec rendition's pattern.
4. Apply fix R6 — replace "~8.9 / 10" with "9.0 / 10" (the exact arithmetic mean) and
   label it explicitly.
5. Apply fix R2 — restore the implementation-sequence block from suggestion S10 under
   the third recommendation, so the work plan is actionable in the rendition itself.
6. Optional: apply R4 — add a one-line note clarifying that "Section N" references
   point to the source spec, not to sections of the review.

After R1-R6, this rendition ships as a clean 9.4 / 10 artifact. After just R1 (the
blocking one), it ships as a clean 9.2.

---

## One closing observation

You now have a complete rules-compliant rendition tree in the making:

```
000_cpp17-reference.md           (the reference, original)
001_the_project_sketch.md        (the sketch, original)
002_spoken_design_and_spec.md    (the spec, original)
002_rules.md                     (rendition of the spec)
003_spoken_design_and_spec.md    (rendition of the spec, alt name)
003_corrected_impl.md            (next: applied fixes)
004_review_and_rating.md         (the review, original)
004_review_rules.md              (this rendition, OR)
002_rules.md                     (this rendition, if kept in spec path)
```

The filename conventions are starting to drift. If the rules-compliant rendition tree
is to be maintained long-term, the project needs a file-naming policy. Recommendation:

- Source artifacts keep their numeric prefix (`000_*`, `001_*`, etc.) in their
  original location.
- Renditions go to `_mydocs/` with a `_rules.md` suffix, but never share a numeric
  prefix with their source. Use sequential numbering: `_mydocs/001_rules.md`,
  `_mydocs/002_rules.md`, etc., as the rendition grows.

Right now `002_rules.md` could be either the rendition of the spec or the rendition of
the review. That's a small thing, but it's exactly the kind of small thing that bites
a project in six months.

---

Send the corrected rendition when ready for a quick pass to confirm the fixes. If you'd
rather move on to `003_corrected_impl.md` (the applied-fixes artifact) instead, that's
also a clean next step, to be re-rated against this spec rather than against the
original sketch.

---

## Reviewer's note on this rendition (006)

This file is the rules-compliant rendition of the re-rating. Per the re-rating's own
R1/R6 findings, note that the prior document `005` carries an unresolved 8.7-vs-9.0
score question; this rendition reports the re-rating's numbers as given and does not
silently "fix" them. Resolving R1 (and R2-R6) is tracked as future work against `005`,
to be applied in a new file per the project's append-only / new-file rules.
