# `spoken` — Line-by-Line Source Documentation

> Status: reference companion to the `spoken/` project.
>
> This document describes every line of the library headers (`include/spoken/*.hpp`),
> the library sources (`src/**/*.cpp`), and the CLI app (`apps/spoken.cpp`). Line
> numbers match the files as built and tested. Unit-test files (`tests/*.cpp`) are not
> covered here; they can be documented on request.
>
> Authoring compliance: `_mydocs/002_rules.md` (lines <= 120 chars, no tabs, no
> GitHub-style pipe-bordered tables).

---

## Index

- Headers: version, error, tokens, tokenizer, number_converter, symbol_converter,
  sentence_builder, conversion_pipeline, tts/tts_engine, tts/null_engine, tts/factory.
- Sources: tokens, tokenizer, number_converter, symbol_converter, sentence_builder,
  conversion_pipeline, tts/null_engine, tts/factory.
- App: apps/spoken.cpp.

---

## include/spoken/version.hpp

- L1 `#pragma once` — include guard: the header is processed once per translation unit.
- L2 (blank).
- L3 `#include <cstdint>` — brings in fixed-width integer types (`std::uint32_t`).
- L4 (blank).
- L5 `namespace spoken {` — opens the library's root namespace.
- L6 `inline namespace v1 {` — opens an inline namespace so names are visible as
  `spoken::name` while leaving room for a future `v2` ABI.
- L7 (blank).
- L8 `inline constexpr std::uint32_t version_major = 1;` — compile-time major version,
  `inline` so the constant has one definition across all translation units.
- L9 `inline constexpr std::uint32_t version_minor = 0;` — compile-time minor version.
- L10 `inline constexpr std::uint32_t version_patch = 0;` — compile-time patch version.
- L11 `inline constexpr const char* version_string = "1.0.0";` — human-readable version
  string for help/banner output.
- L12 (blank).
- L13 `}  // namespace v1` — closes the inline namespace.
- L14 `}  // namespace spoken` — closes the root namespace.

---

## include/spoken/error.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <stdexcept>` — base classes `std::runtime_error`/`std::exception`.
- L4 `#include <string>` — `std::string` used by the inherited message constructors.
- L6 `namespace spoken {` — opens the root namespace.
- L8 `class SpokenError : public std::runtime_error {` — root of the library's
  exception hierarchy; catch this to catch any library error.
- L9 `public:` — begins the public interface.
- L10 `using std::runtime_error::runtime_error;` — inherits the base constructors so
  `SpokenError{"msg"}` works without re-declaring them.
- L11 `};` — closes `SpokenError`.
- L13 `class InvalidInputError : public SpokenError {` — thrown for malformed input
  (e.g. an unparseable number).
- L14 `public:` — public section.
- L15 `using SpokenError::SpokenError;` — inherits the message constructor.
- L16 `};` — closes `InvalidInputError`.
- L18 `class TtsError : public SpokenError {` — thrown by TTS engines/factory on
  failure.
- L19 `public:` — public section.
- L20 `using SpokenError::SpokenError;` — inherits the message constructor.
- L21 `};` — closes `TtsError`.
- L23 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/tokens.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <cstdint>` — `std::int64_t` for the integer token.
- L4 `#include <iosfwd>` — forward declarations of stream types for `operator<<`.
- L5 `#include <string>` — `std::string` token fields.
- L6 `#include <variant>` — `std::variant` that backs the `Token` sum type.
- L8 `namespace spoken {` — opens the namespace.
- L10 `struct Word { std::string text; };` — token holding a word's text.
- L11 `struct Integer { std::int64_t value; std::string lexeme; };` — integer value
  plus its original source text (`lexeme`, added for Q-4).
- L12 `struct Decimal { double value; std::string lexeme; };` — decimal value plus the
  original literal, which drives exact digit-by-digit speech (Q-4/DEF-4).
- L13 `struct Symbol { std::string glyph; };` — one or two-character symbol/operator.
- L14 `struct Punctuation { char mark; };` — a single punctuation mark.
- L15 `struct Whitespace { std::string spaces; };` — a run of whitespace, preserved by
  the tokenizer (later dropped by the pipeline, Q-1).
- L17 `using Token =` — names the variant type.
- L18 `std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;` — the
  type-safe sum type that holds exactly one alternative at a time.
- L20 `std::ostream& operator<<(std::ostream& os, const Token& t);` — declares stream
  insertion for tokens (used in diagnostics); defined in tokens.cpp.
- L22 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/tokenizer.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include "spoken/tokens.hpp"` — needs the `Token` type it produces.
- L5 `#include <string_view>` — input is a non-owning `std::string_view`.
- L6 `#include <vector>` — output is a `std::vector<Token>`.
- L8 `namespace spoken {` — opens the namespace.
- L10 `class Tokenizer {` — the stateless lexer type.
- L11 `public:` — public interface.
- L12 `[[nodiscard]] std::vector<Token> tokenize(std::string_view text) const;` —
  converts text to tokens; `[[nodiscard]]` warns if the result is ignored.
- L13 `};` — closes `Tokenizer`.
- L15 `[[nodiscard]] inline std::vector<Token> tokenize(std::string_view text) {` —
  free-function convenience wrapper (the tokenizer is stateless).
- L16 `return Tokenizer{}.tokenize(text);` — constructs a temporary tokenizer and
  delegates to it.
- L17 `}` — closes the free function.
- L19 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/number_converter.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <array>` — `std::array` lookup tables.
- L4 `#include <cstdint>` — `std::int64_t`.
- L5 `#include <string>` — return type.
- L6 `#include <string_view>` — table element type and the lexeme parameter.
- L8 `namespace spoken {` — opens the namespace.
- L10 `class NumberConverter {` — pure, stateless integer/decimal-to-words converter.
- L11 `public:` — public interface.
- L12 `[[nodiscard]] std::string to_words(std::int64_t value) const;` — converts an
  integer to English words.
- L14 `[[nodiscard]] std::string decimal_to_words(std::string_view lexeme) const;` —
  converts a decimal from its original literal (Q-4/DEF-4).
- L16 `private:` — internal lookup tables follow.
- L17 `static constexpr std::array<std::string_view, 20> kOnes = {` — words for 0..19.
- L18-L21 (table entries) — the 20 number words "zero".."nineteen".
- L22 `static constexpr std::array<std::string_view, 10> kTens = {` — tens words.
- L23-L24 (entries) — "", "", "twenty".."ninety" (indices 0/1 unused).
- L25 `static constexpr std::array<std::string_view, 7> kIllions = {` — scale words.
- L26-L27 (entries) — "", "thousand".."quintillion".
- L28 `};` — closes the table block / class body trailing members.
- L30 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/symbol_converter.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <string>` — return type and map value.
- L4 `#include <string_view>` — query parameter type.
- L5 `#include <unordered_map>` — the glyph-to-words table.
- L7 `namespace spoken {` — opens the namespace.
- L9 `class SymbolConverter {` — maps symbols/operators to spoken words.
- L10 `public:` — public interface.
- L11 `SymbolConverter();` — constructor builds the lookup table.
- L13 `[[nodiscard]] std::string to_words(std::string_view glyph) const;` — converts a
  glyph to words (with a spell-out fallback).
- L14 `[[nodiscard]] bool known(std::string_view glyph) const noexcept;` — reports
  whether the glyph is in the table; `noexcept` as it cannot throw.
- L16 `private:` — internal state.
- L17 `std::unordered_map<std::string, std::string> table_;` — the glyph table; the map
  manages its own memory (Rule of 0).
- L18 `};` — closes the class.
- L20 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/sentence_builder.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <string>` — return type.
- L4 `#include <vector>` — input is a vector of spoken forms.
- L6 `namespace spoken {` — opens the namespace.
- L8 `class SentenceBuilder {` — joins per-token spoken forms into a sentence.
- L9 `public:` — public interface.
- L10 `[[nodiscard]] std::string assemble(` — starts the method declaration.
- L11 `const std::vector<std::string>& spoken_per_token) const;` — takes the forms by
  const reference (no copy) and returns the assembled sentence.
- L12 `};` — closes the class.
- L14 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/conversion_pipeline.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include "spoken/number_converter.hpp"` — pipeline owns a NumberConverter.
- L4 `#include "spoken/sentence_builder.hpp"` — owns a SentenceBuilder.
- L5 `#include "spoken/symbol_converter.hpp"` — owns a SymbolConverter.
- L6 `#include "spoken/tokenizer.hpp"` — uses the tokenizer.
- L8 `#include <string>` — return type.
- L9 `#include <string_view>` — input type.
- L11 `namespace spoken {` — opens the namespace.
- L13 `class ConversionPipeline {` — orchestrates tokenize -> convert -> assemble.
- L14 `public:` — public interface.
- L15 `ConversionPipeline();` — constructor (defaulted in the .cpp).
- L17 `[[nodiscard]] std::string convert(std::string_view text) const;` — the
  end-to-end conversion entry point.
- L19 `private:` — owned collaborators (composition, Rule of 0).
- L20 `NumberConverter numbers_;` — number-to-words converter.
- L21 `SymbolConverter symbols_;` — symbol-to-words converter.
- L22 `SentenceBuilder builder_;` — final sentence assembler.
- L23 `};` — closes the class.
- L25 `}  // namespace spoken` — closes the namespace.

---

## include/spoken/tts/tts_engine.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <string>` — `name()` return and `to_wav` path.
- L4 `#include <string_view>` — text parameters.
- L6 `namespace spoken::tts {` — opens the nested TTS namespace.
- L8-L9 (comment) — documents the interface and the "may throw TtsError" contract.
- L10 `class TtsEngine {` — the abstract engine interface (Strategy/DIP hook).
- L11 `public:` — public interface.
- L12 `virtual ~TtsEngine() = default;` — virtual destructor so derived engines are
  destroyed correctly via a base pointer.
- L14 `virtual void speak(std::string_view text) = 0;` — pure virtual: play text aloud.
- L15 `virtual void to_wav(std::string_view text, const std::string& wav_path) = 0;` —
  pure virtual: synthesize text to a WAV file.
- L16 `[[nodiscard]] virtual std::string name() const = 0;` — pure virtual: engine id.
- L17 `};` — closes the interface.
- L19 `}  // namespace spoken::tts` — closes the namespace.

---

## include/spoken/tts/null_engine.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include "spoken/tts/tts_engine.hpp"` — base interface implemented here.
- L5 `#include <functional>` — `std::function` sink type.
- L6 `#include <string>` — `name()`/path types.
- L7 `#include <string_view>` — text parameters.
- L9 `namespace spoken::tts {` — opens the namespace.
- L11 (comment) — explains this engine emits text to a sink instead of audio.
- L12 `class NullEngine final : public TtsEngine {` — concrete no-audio engine; `final`
  forbids further derivation.
- L13 `public:` — public interface.
- L14 `using Sink = std::function<void(std::string_view)>;` — type alias for the
  text-receiving callback.
- L15 `explicit NullEngine(Sink on_text);` — constructor taking the sink; `explicit`
  prevents implicit conversions.
- L17 `void speak(std::string_view text) override;` — implements speak by calling sink.
- L18 `void to_wav(std::string_view text, const std::string& wav_path) override;` —
  implements WAV output as a placeholder text file.
- L19 `[[nodiscard]] std::string name() const override { return "null"; }` — returns
  the engine id "null" inline.
- L21 `private:` — internal state.
- L22 `Sink sink_;` — the stored callback.
- L23 `};` — closes the class.
- L25 `}  // namespace spoken::tts` — closes the namespace.

---

## include/spoken/tts/factory.hpp

- L1 `#pragma once` — include-once guard.
- L3 `#include <memory>` — `std::unique_ptr` return type.
- L4 `#include <string>` — config string fields.
- L6 `#include "spoken/tts/null_engine.hpp"` — a possible product.
- L7 `#include "spoken/tts/tts_engine.hpp"` — the returned interface type.
- L9 `#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK` — compile the eSpeak path
  only when the build enabled it.
- L10 `#include "spoken/tts/espeak_engine.hpp"` — real engine header (guarded).
- L11 `#endif` — ends the guard.
- L13 `namespace spoken::tts {` — opens the namespace.
- L15 `enum class EngineKind { Auto, Espeak, Null };` — scoped enum of engine choices.
- L17 `struct EngineConfig {` — configuration passed to the factory.
- L18 `EngineKind kind = EngineKind::Auto;` — which engine (Auto picks at runtime).
- L19 `std::string voice = "en";` — requested voice.
- L20 `int speed_wpm = 175;` — speech rate in words per minute.
- L21 `std::string text_sink;` — optional file path for null-engine text output.
- L22 `};` — closes `EngineConfig`.
- L24 `[[nodiscard]] std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg);` —
  factory function returning an owned engine via the base interface.
- L26 `}  // namespace spoken::tts` — closes the namespace.

---

## src/tokens.cpp

- L1 `#include "spoken/tokens.hpp"` — declarations for the `Token` type/operator.
- L3 `#include <ostream>` — full `std::ostream` definition for `<<`.
- L4 `#include <type_traits>` — `std::decay_t`/`std::is_same_v` for the visitor.
- L6 `namespace spoken {` — opens the namespace.
- L8 `std::ostream& operator<<(std::ostream& os, const Token& t) {` — defines token
  stream insertion.
- L9 `std::visit([&os](const auto& v) {` — visits the active alternative with a generic
  lambda capturing the stream by reference.
- L10 `using T = std::decay_t<decltype(v)>;` — the concrete alternative type, stripped
  of reference/const.
- L11 `if constexpr (std::is_same_v<T, Word>) {` — compile-time branch for `Word`.
- L12 `os << "Word(" << v.text << ')';` — prints the word text.
- L13 `} else if constexpr (std::is_same_v<T, Integer>) {` — branch for `Integer`.
- L14 `os << "Integer(" << v.value << ')';` — prints the integer value.
- L15 `} else if constexpr (std::is_same_v<T, Decimal>) {` — branch for `Decimal`.
- L16 `os << "Decimal(" << v.value << ')';` — prints the decimal value.
- L17 `} else if constexpr (std::is_same_v<T, Symbol>) {` — branch for `Symbol`.
- L18 `os << "Symbol(" << v.glyph << ')';` — prints the glyph.
- L19 `} else if constexpr (std::is_same_v<T, Punctuation>) {` — branch for punctuation.
- L20 `os << "Punct(" << v.mark << ')';` — prints the punctuation mark.
- L21 `} else if constexpr (std::is_same_v<T, Whitespace>) {` — branch for whitespace.
- L22 `os << "WS(\"" << v.spaces << "\")";` — prints the whitespace run quoted.
- L23 `}` — closes the if-constexpr chain.
- L24 `}, t);` — ends the lambda and applies `std::visit` to token `t`.
- L25 `return os;` — returns the stream for chaining.
- L26 `}` — closes `operator<<`.
- L28 `}  // namespace spoken` — closes the namespace.

---

## src/tokenizer.cpp

- L1 `#include "spoken/tokenizer.hpp"` — the class being defined.
- L2 `#include "spoken/error.hpp"` — `InvalidInputError` for bad numbers.
- L4 `#include <algorithm>` — `std::remove` for stripping separators.
- L5 `#include <cctype>` — character classification (`isalpha`, etc.).
- L6 `#include <string>` — string building and `std::stoll`/`std::stod`.
- L8 `namespace spoken {` — opens the namespace.
- L9 `namespace {` — anonymous namespace for file-local helpers.
- L11 `bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }` — ASCII digit
  test (locale-independent).
- L12-L14 `bool is_alpha(char c) noexcept {` ... — letter test via `std::isalpha` with a
  cast to `unsigned char` to avoid UB on negative chars.
- L15-L17 `bool is_space(char c) noexcept {` ... — whitespace test via `std::isspace`.
- L18-L20 `bool is_punct(char c) noexcept {` ... — punctuation test via `std::ispunct`.
- L22 `}  // namespace` — closes the anonymous namespace.
- L24 `std::vector<Token> Tokenizer::tokenize(std::string_view text) const {` — begins
  the tokenizer definition.
- L25 `std::vector<Token> out;` — accumulates the produced tokens.
- L26 `out.reserve(text.size() / 4 + 1);` — pre-sizes to reduce reallocations (rough
  estimate of token count).
- L28 `std::size_t i = 0;` — current scan index.
- L29 `while (i < text.size()) {` — loops until the input is consumed.
- L30 `const char c = text[i];` — current character.
- L32 `if (is_space(c)) {` — whitespace run branch.
- L33 `std::size_t j = i;` — start of the run.
- L34 `while (j < text.size() && is_space(text[j])) ++j;` — extends to the run's end.
- L35 `out.emplace_back(Whitespace{std::string{text.substr(i, j - i)}});` — emits a
  Whitespace token with the exact spaces.
- L36 `i = j;` — advances past the run.
- L37 `} else if (is_digit(c)) {` — number branch.
- L38 `std::size_t j = i;` — start of the number.
- L39 `bool seen_dot = false;` — tracks whether a decimal point appeared.
- L40 `while (j < text.size() &&` — continues while the next char is part of a number...
- L41 `(is_digit(text[j]) || text[j] == '.' || text[j] == ',')) {` — digit, dot, or
  thousands comma.
- L42 `if (text[j] == '.') seen_dot = true;` — note a decimal point.
- L43 `++j;` — advance.
- L44 `}` — ends the number scan.
- L45 `std::string lexeme{text.substr(i, j - i)};` — keeps the original literal (Q-4).
- L46 `std::string num = lexeme;` — a working copy to normalize.
- L47 `num.erase(std::remove(num.begin(), num.end(), ','), num.end());` — strips
  thousands separators (erase-remove idiom).
- L48 `try {` — guards numeric parsing.
- L49 `if (seen_dot) {` — decimal vs integer.
- L50 `out.emplace_back(Decimal{std::stod(num), lexeme});` — emits a Decimal with parsed
  value and original lexeme.
- L51 `} else {` — integer case.
- L52 `out.emplace_back(Integer{std::stoll(num), lexeme});` — emits an Integer with
  value and lexeme.
- L53 `}` — ends decimal/integer if.
- L54 `} catch (const std::exception&) {` — catches parse failures (overflow, etc.).
- L55 `throw InvalidInputError{"cannot parse number: " + num};` — rethrows as a typed
  library error.
- L56 `}` — ends the try/catch.
- L57 `i = j;` — advance past the number.
- L58 `} else if (is_alpha(c)) {` — word branch.
- L59 `std::size_t j = i;` — start of the word.
- L60 `while (j < text.size() &&` — continues while word characters follow...
- L61 `(is_alpha(text[j]) || text[j] == '\'' || text[j] == '-')) {` — letters,
  apostrophes, and hyphens (for contractions/compounds).
- L62 `++j;` — advance.
- L63 `}` — ends the word scan.
- L64 `out.emplace_back(Word{std::string{text.substr(i, j - i)}});` — emits the Word.
- L65 `i = j;` — advance.
- L66 `} else if (is_punct(c)) {` — punctuation/symbol branch.
- L67 `std::size_t j = i + 1;` — tentatively consume one char.
- L68 `if (j < text.size() && is_punct(text[j])) ++j;` — greedily grab a second punct
  char for two-character operators (e.g. "!=").
- L69 `std::string glyph{text.substr(i, j - i)};` — the captured 1-2 char glyph.
- L70 `if (glyph.size() == 1 &&` — a single char that is real punctuation...
- L71 `std::string{".!,?;:"}.find(c) != std::string::npos) {` — is one of the sentence
  marks.
- L72 `out.emplace_back(Punctuation{c});` — emit a Punctuation token.
- L73 `} else {` — otherwise it is a symbol/operator.
- L74 `out.emplace_back(Symbol{std::move(glyph)});` — emit a Symbol (moving the string).
- L75 `}` — ends the punct/symbol if.
- L76 `i = j;` — advance past the glyph.
- L77 `} else {` — any other byte (non-space, non-alnum, non-punct).
- L78 `out.emplace_back(Symbol{std::string(1, c)});` — emit it as a single-char Symbol.
- L79 `++i;` — advance one byte.
- L80 `}` — ends the branch chain.
- L81 `}` — ends the while loop.
- L82 `return out;` — returns the token list.
- L83 `}` — closes the function.
- L85 `}  // namespace spoken` — closes the namespace.

---

## src/number_converter.cpp

- L1 `#include "spoken/number_converter.hpp"` — the class being defined.
- L3 `#include <algorithm>` — `std::remove` in the decimal path.
- L4 `#include <cstdio>` — retained for portability of formatting helpers.
- L5 `#include <string>` — `std::stoll` and string building.
- L7 `namespace spoken {` — opens the namespace.
- L8 `namespace {` — file-local helper.
- L10 `std::string three_digit(std::uint64_t n,` — renders 0..999 as words; takes...
- L11 `const std::array<std::string_view, 20>& ones,` — the ones/teens table...
- L12 `const std::array<std::string_view, 10>& tens) {` — and the tens table.
- L13 `std::string out;` — accumulates the words.
- L14 `if (n >= 100) {` — hundreds present.
- L15 `out += std::string{ones[n / 100]};` — leading digit word.
- L16 `out += " hundred";` — the word "hundred".
- L17 `n %= 100;` — drop the hundreds digit.
- L18 `if (n) out += ' ';` — separator if a remainder follows.
- L19 `}` — ends the hundreds block.
- L20 `if (n >= 20) {` — tens-and-ones case (20..99).
- L21 `out += std::string{tens[n / 10]};` — tens word.
- L22 `if (n % 10) { out += '-'; out += std::string{ones[n % 10]}; }` — hyphen + ones
  word (e.g. "forty-two").
- L23 `} else if (n > 0) {` — 1..19 case.
- L24 `out += std::string{ones[n]};` — direct lookup of the word.
- L25 `}` — ends the tens/ones block.
- L26 `return out;` — returns the 0..999 rendering.
- L27 `}` — closes `three_digit`.
- L29 `}  // namespace` — closes the anonymous namespace.
- L31 `std::string NumberConverter::to_words(std::int64_t value) const {` — integer
  renderer.
- L32 `if (value == 0) return std::string{kOnes[0]};` — fast path for zero.
- L34 `const bool negative = value < 0;` — remembers the sign.
- L35 `std::uint64_t n = negative` — compute the magnitude as unsigned...
- L36 `? (~static_cast<std::uint64_t>(value) + 1ULL)` — two's-complement negation that
  is safe even for `INT64_MIN` (DEF-3 correctness).
- L37 `: static_cast<std::uint64_t>(value);` — non-negative case.
- L39 `std::string magnitude;` — built separately so the "negative" prefix is not lost
  (DEF-3 fix).
- L40 `int chunk_idx = 0;` — index into the scale words (thousand, million, ...).
- L41 `bool any = false;` — whether any chunk has been emitted yet.
- L42 `while (n > 0 && chunk_idx < static_cast<int>(kIllions.size())) {` — processes the
  number in 3-digit chunks within the supported scale range.
- L43 `std::uint64_t chunk = n % 1000;` — the lowest three digits.
- L44 `if (chunk != 0) {` — skip empty chunks.
- L45 `std::string part = three_digit(chunk, kOnes, kTens);` — words for this chunk.
- L46 `if (chunk_idx > 0) {` — append the scale word for non-lowest chunks.
- L47 `part += ' ';` — separator.
- L48 `part += std::string{kIllions[chunk_idx]};` — e.g. "thousand"/"million".
- L49 `}` — ends scale-word append.
- L50 `if (any) { part += ' '; part += magnitude; }` — prepend this higher chunk before
  the previously built lower part.
- L51 `magnitude = std::move(part);` — adopt the combined string.
- L52 `any = true;` — mark that output exists.
- L53 `}` — ends non-empty-chunk block.
- L54 `n /= 1000;` — move to the next chunk.
- L55 `++chunk_idx;` — advance the scale index.
- L56 `}` — ends the chunk loop.
- L58 `return negative ? ("negative " + magnitude) : magnitude;` — prepend the sign word
  once, at the end (DEF-3 fix).
- L59 `}` — closes `to_words(int64)`.
- L61 `std::string NumberConverter::decimal_to_words(std::string_view lexeme) const {` —
  decimal renderer driven by the original literal.
- L62 `if (lexeme.empty()) return std::string{kOnes[0]};` — empty lexeme -> "zero".
- L64 `bool negative = false;` — sign flag.
- L65 `std::size_t start = 0;` — index where digits begin.
- L66 `if (lexeme[0] == '-') { negative = true; start = 1; }` — handle a leading minus.
- L68 `const std::size_t dot = lexeme.find('.', start);` — locate the decimal point.
- L69 `std::string int_digits{lexeme.substr(` — extract the integer-part digits...
- L70 `start, dot == std::string_view::npos ? std::string_view::npos` — to end if no
  dot...
- L71 `: dot - start)};` — otherwise up to the dot.
- L72 `int_digits.erase(std::remove(int_digits.begin(), int_digits.end(), ','),` —
  strip thousands commas...
- L73 `int_digits.end());` — completing the erase-remove.
- L74 `if (int_digits.empty()) int_digits = "0";` — treat ".5" as "0.5".
- L76 `std::string out;` — output accumulator.
- L77 `if (negative) out += "negative ";` — sign word first.
- L78 `out += to_words(static_cast<std::int64_t>(std::stoll(int_digits)));` — speak the
  integer part via the integer renderer.
- L80 `if (dot != std::string_view::npos) {` — if there is a fractional part...
- L81 `out += " point";` — the word "point".
- L82 `for (std::size_t k = dot + 1; k < lexeme.size(); ++k) {` — iterate fractional
  characters of the lexeme.
- L83 `const char d = lexeme[k];` — current fractional character.
- L84 `if (d >= '0' && d <= '9') {` — only speak real digits (DEF-4: no zero padding).
- L85 `out += ' ';` — separator.
- L86 `out += std::string{kOnes[static_cast<std::size_t>(d - '0')]};` — the digit word.
- L87 `}` — ends digit check.
- L88 `}` — ends fractional loop.
- L89 `}` — ends fractional-part block.
- L90 `return out;` — returns the spoken decimal.
- L91 `}` — closes `decimal_to_words`.
- L93 `}  // namespace spoken` — closes the namespace.

---

## src/symbol_converter.cpp

- L1 `#include "spoken/symbol_converter.hpp"` — the class being defined.
- L3 `namespace spoken {` — opens the namespace.
- L5 `SymbolConverter::SymbolConverter()` — constructor.
- L6 `: table_{` — initializes the glyph map via its initializer list.
- L7-L23 (entries) — map each symbol/operator to its spoken form, e.g. `{"$",
  "dollars"}`, `{"%", "percent"}`, `{"!=", "not equals"}`, `{"->", "arrow"}`.
- L24 `} {}` — closes the initializer list and the empty constructor body.
- L26 `std::string SymbolConverter::to_words(std::string_view glyph) const {` — lookup
  with fallback.
- L27 `auto it = table_.find(std::string{glyph});` — search the table by glyph.
- L28 `if (it != table_.end()) return it->second;` — return the known word if found.
- L30 `std::string out;` — fallback accumulator (spell out unknown glyphs).
- L31 `for (char c : glyph) {` — iterate each character.
- L32 `if (!out.empty()) out += ' ';` — space between spelled characters.
- L33 `switch (c) {` — name common special characters.
- L34-L39 (cases) — `#`->hash, `~`->tilde, backtick, `^`->caret, `|`->pipe,
  backslash.
- L40 `default: out += c; break;` — otherwise keep the character itself.
- L41 `}` — ends the switch.
- L42 `}` — ends the loop.
- L43 `return out;` — returns the spelled-out fallback.
- L44 `}` — closes `to_words`.
- L46 `bool SymbolConverter::known(std::string_view glyph) const noexcept {` — membership
  test.
- L47 `return table_.find(std::string{glyph}) != table_.end();` — true iff the glyph is
  in the table.
- L48 `}` — closes `known`.
- L50 `}  // namespace spoken` — closes the namespace.

---

## src/sentence_builder.cpp

- L1 `#include "spoken/sentence_builder.hpp"` — the class being defined.
- L3 `#include <algorithm>` — `std::find_if` for trimming.
- L4 `#include <cctype>` — `std::isspace`/`std::toupper`.
- L5 `#include <string>` — string building.
- L7 `namespace spoken {` — opens the namespace.
- L8 `namespace {` — file-local helpers.
- L10 `bool is_blank(const std::string& s) {` — true if a form is empty/all-whitespace.
- L11 `for (char c : s) {` — scan characters.
- L12 `if (!std::isspace(static_cast<unsigned char>(c))) return false;` — any non-space
  means not blank.
- L13 `}` — ends loop.
- L14 `return true;` — all whitespace (or empty).
- L15 `}` — closes `is_blank`.
- L17 `bool is_punct_form(const std::string& s) {` — true if the form is a single
  sentence-punctuation char.
- L18 `if (s.size() != 1) return false;` — must be exactly one char.
- L19 `return std::string{".,!?;:"}.find(s[0]) != std::string::npos;` — and a known
  punctuation mark.
- L20 `}` — closes `is_punct_form`.
- L22 `}  // namespace` — closes the anonymous namespace.
- L24 `std::string SentenceBuilder::assemble(` — the assembler definition.
- L25 `const std::vector<std::string>& spoken_per_token) const {` — input forms by const
  reference.
- L27 `std::string out;` — output sentence accumulator.
- L28 `for (const auto& form : spoken_per_token) {` — iterate each spoken form.
- L29 `if (form.empty() || is_blank(form)) continue;` — drop empty/whitespace forms
  (Q-1).
- L30 `if (is_punct_form(form)) {` — punctuation marks...
- L31 `out += form;` — attach with no leading space (DEF-2 fix).
- L32 `} else {` — words/numbers/symbols.
- L33 `if (!out.empty()) out += ' ';` — single-space separator between tokens.
- L34 `out += form;` — append the form.
- L35 `}` — ends punct/word branch.
- L36 `}` — ends the loop.
- L38 `auto not_ws = [](char c) {` — predicate: a non-whitespace character...
- L39 `return !std::isspace(static_cast<unsigned char>(c));` — used to find content
  bounds.
- L40 `};` — ends the lambda.
- L41 `out.erase(out.begin(), std::find_if(out.begin(), out.end(), not_ws));` — trim
  leading whitespace.
- L42 `out.erase(std::find_if(out.rbegin(), out.rend(), not_ws).base(), out.end());` —
  trim trailing whitespace (reverse search, convert iterator with `.base()`).
- L44 `if (!out.empty()) {` — only finalize non-empty sentences.
- L45 `out[0] = static_cast<char>(` — capitalize the first character...
- L46 `std::toupper(static_cast<unsigned char>(out[0])));` — via `toupper` with a safe
  cast.
- L47 `const char last = out.back();` — inspect the final character.
- L48 `if (last != '.' && last != '?' && last != '!') out += '.';` — ensure terminal
  punctuation.
- L49 `}` — ends the finalize block.
- L50 `return out;` — returns the assembled sentence.
- L51 `}` — closes `assemble`.
- L53 `}  // namespace spoken` — closes the namespace.

---

## src/conversion_pipeline.cpp

- L1 `#include "spoken/conversion_pipeline.hpp"` — the class being defined.
- L3 `#include <algorithm>` — `std::transform`.
- L4 `#include <iterator>` — `std::back_inserter`.
- L5 `#include <string>` — string handling.
- L6 `#include <type_traits>` — `std::decay_t`/`std::is_same_v` in the visitor.
- L7 `#include <vector>` — intermediate vector of forms.
- L9 `namespace spoken {` — opens the namespace.
- L11 `ConversionPipeline::ConversionPipeline() = default;` — defaulted constructor
  (members default-construct; Rule of 0).
- L13 `namespace {` — file-local helper.
- L15 `std::string token_to_spoken(const Token& t,` — converts one token to its spoken
  form; takes...
- L16 `const NumberConverter& nc,` — the number converter...
- L17 `const SymbolConverter& sc) {` — and the symbol converter.
- L18 `return std::visit([&](const auto& v) -> std::string {` — visit the active
  alternative, returning a string.
- L19 `using T = std::decay_t<decltype(v)>;` — the alternative's concrete type.
- L20 `if constexpr (std::is_same_v<T, Word>) {` — word case.
- L21 `return v.text;` — speak the word as-is.
- L22 `} else if constexpr (std::is_same_v<T, Integer>) {` — integer case.
- L23 `return nc.to_words(v.value);` — convert via the integer renderer.
- L24 `} else if constexpr (std::is_same_v<T, Decimal>) {` — decimal case.
- L25 `return nc.decimal_to_words(v.lexeme);` — render from the original lexeme (Q-4).
- L26 `} else if constexpr (std::is_same_v<T, Symbol>) {` — symbol case.
- L27 `return sc.to_words(v.glyph);` — map the glyph to words.
- L28 `} else if constexpr (std::is_same_v<T, Punctuation>) {` — punctuation case.
- L29 `return std::string{v.mark};` — pass the mark through as a one-char string.
- L30 `} else if constexpr (std::is_same_v<T, Whitespace>) {` — whitespace case.
- L31 `return std::string{};` — drop whitespace (Q-1); the builder owns spacing.
- L32 `}` — ends the if-constexpr chain.
- L33 `}, t);` — applies the visitor to token `t`.
- L34 `}` — closes `token_to_spoken`.
- L36 `}  // namespace` — closes the anonymous namespace.
- L38 `std::string ConversionPipeline::convert(std::string_view text) const {` — the
  end-to-end conversion.
- L39 `const auto tokens = spoken::tokenize(text);` — step 1: tokenize the input.
- L41 `std::vector<std::string> spoken_per_token;` — holds per-token spoken forms.
- L42 `spoken_per_token.reserve(tokens.size());` — pre-size to avoid reallocation.
- L43 `std::transform(tokens.begin(), tokens.end(),` — step 2: map each token...
- L44 `std::back_inserter(spoken_per_token),` — appending results to the vector...
- L45 `[&](const Token& t) {` — via a lambda capturing the converters...
- L46 `return token_to_spoken(t, numbers_, symbols_);` — that converts one token.
- L47 `});` — ends transform.
- L49 `return builder_.assemble(spoken_per_token);` — step 3: assemble the sentence.
- L50 `}` — closes `convert`.
- L52 `}  // namespace spoken` — closes the namespace.

---

## src/tts/null_engine.cpp

- L1 `#include "spoken/tts/null_engine.hpp"` — the class being defined.
- L2 `#include "spoken/error.hpp"` — `TtsError` for failures.
- L4 `#include <fstream>` — file output in `to_wav`.
- L5 `#include <ostream>` — stream insertion.
- L6 `#include <utility>` — `std::move`.
- L8 `namespace spoken::tts {` — opens the namespace.
- L10 `NullEngine::NullEngine(Sink on_text) : sink_{std::move(on_text)} {` — stores the
  sink (moved in).
- L11 `if (!sink_) throw TtsError{"NullEngine needs a non-empty sink"};` — rejects an
  empty callback.
- L12 `}` — closes the constructor.
- L14 `void NullEngine::speak(std::string_view text) {` — "speak" implementation.
- L15 `sink_(text);` — forwards the text to the sink.
- L16 `}` — closes `speak`.
- L18 `void NullEngine::to_wav(std::string_view text, const std::string& wav_path) {` —
  placeholder WAV output.
- L19 `std::ofstream out{wav_path};` — opens the target file.
- L20 `if (!out) throw TtsError{"cannot open " + wav_path};` — error if it cannot open.
- L21 `out << "# spoken null TTS, audio would be: \"" << text << "\"\n";` — writes a
  human-readable placeholder line.
- L22 `}` — closes `to_wav`.
- L24 `}  // namespace spoken::tts` — closes the namespace.

---

## src/tts/factory.cpp

- L1 `#include "spoken/tts/factory.hpp"` — the factory declaration.
- L2 `#include "spoken/error.hpp"` — `TtsError`.
- L4 `#include <fstream>` — file sink for the null engine.
- L5 `#include <iostream>` — `std::cout` default sink (DEF-7 fix: missing include).
- L6 `#include <memory>` — `std::make_unique`/`std::make_shared`.
- L7 `#include <ostream>` — stream type for the sink.
- L8 `#include <utility>` — `std::move`.
- L10 `namespace spoken::tts {` — opens the namespace.
- L11 `namespace {` — file-local helper.
- L13 `constexpr bool espeak_available() noexcept {` — compile-time availability check.
- L14 `#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK` — when built with eSpeak...
- L15 `return true;` — engine is available (DEF-7: no `std::system` probe).
- L16 `#else` — otherwise...
- L17 `return false;` — not available.
- L18 `#endif` — ends the guard.
- L19 `}` — closes the helper.
- L21 `}  // namespace` — closes the anonymous namespace.
- L23 `std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg) {` — the factory.
- L24 `EngineKind kind = cfg.kind;` — copy the requested kind.
- L25 `if (kind == EngineKind::Auto) {` — resolve Auto at runtime...
- L26 `kind = espeak_available() ? EngineKind::Espeak : EngineKind::Null;` — eSpeak if
  available, else Null.
- L27 `}` — ends Auto resolution.
- L29 `switch (kind) {` — build the chosen engine.
- L30 `case EngineKind::Espeak: {` — real engine branch.
- L31 `#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK` — only when compiled in...
- L32 `auto eng = std::make_unique<EspeakEngine>();` — construct the eSpeak engine.
- L33 `eng->set_voice(cfg.voice);` — apply the voice.
- L34 `eng->set_speed(cfg.speed_wpm);` — apply the speed.
- L35 `return eng;` — return it.
- L36 `#else` — when not compiled in...
- L37 `throw TtsError{"eSpeak requested but not built in"};` — fail clearly.
- L38 `#endif` — ends the guard.
- L39 `}` — ends the Espeak case.
- L40 `case EngineKind::Null: {` — null engine branch.
- L41 `std::ostream* out = &std::cout;` — default text destination is stdout.
- L42-L43 (comment) — explains why a shared_ptr is needed for a copyable sink.
- L44 `std::shared_ptr<std::ofstream> file;` — optional file owner, shared so the sink
  lambda stays copyable (fixes the `std::function` copy-constructible requirement).
- L45 `if (!cfg.text_sink.empty()) {` — if a file path was configured...
- L46 `file = std::make_shared<std::ofstream>(cfg.text_sink);` — open it.
- L47 `if (!*file) throw TtsError{"cannot open " + cfg.text_sink};` — error if it fails.
- L48 `out = file.get();` — point the sink at the file stream.
- L49 `}` — ends the file-sink setup.
- L50 `auto sink = [out, file](std::string_view text) {` — sink lambda capturing the
  target pointer and (copyable) shared file handle.
- L51 `*out << text << '\n';` — writes one line per spoken text.
- L52 `};` — ends the lambda.
- L53 `return std::make_unique<NullEngine>(std::move(sink));` — build the null engine.
- L54 `}` — ends the Null case.
- L55 `case EngineKind::Auto:` — Auto was resolved above...
- L56 `break;` — unreachable fall-through to the throw below.
- L57 `}` — ends the switch.
- L58 `throw TtsError{"unknown engine kind"};` — safety net for an unexpected kind.
- L59 `}` — closes `make_engine`.
- L61 `}  // namespace spoken::tts` — closes the namespace.

---

## apps/spoken.cpp

### Includes and namespace

- L1 (comment) — file banner: the corrected CLI entry point.
- L2 `#include "spoken/conversion_pipeline.hpp"` — the conversion pipeline.
- L3 `#include "spoken/error.hpp"` — typed exceptions for exit-code mapping.
- L4 `#include "spoken/tts/factory.hpp"` — engine factory.
- L5 `#include "spoken/tts/tts_engine.hpp"` — engine interface.
- L6 `#include "spoken/version.hpp"` — version string for help.
- L8 `#include <algorithm>` — `std::sort` for batch ordering.
- L9 `#include <atomic>` — atomic progress counter.
- L10 `#include <chrono>` — batch timing.
- L11 `#include <filesystem>` — paths and directory iteration.
- L12 `#include <fstream>` — file IO helpers.
- L13 `#include <future>` — `std::async`/`std::future` for batch parallelism.
- L14 `#include <iostream>` — console IO.
- L15 `#include <mutex>` — serialize engine calls in batch (DEF-10).
- L16 `#include <optional>` — optional arguments/results.
- L17 `#include <sstream>` — read stdin/files into strings.
- L18 `#include <string>` / L19 `<string_view>` — text types.
- L20 `#include <thread>` — hardware-concurrency query.
- L21 `#include <vector>` — collections.
- L23 `namespace fs = std::filesystem;` — short alias.
- L25 `namespace {` — file-local (internal-linkage) helpers and types.

### Args and help

- L27 `struct Args {` — parsed command-line options.
- L28 `std::vector<std::string> text;` — positional text words.
- L29 `std::optional<fs::path> file;` — `--file` input path.
- L30 `std::optional<fs::path> out;` — `--out` output path.
- L31 `std::optional<fs::path> wav;` — `--wav` output path.
- L32 `std::optional<fs::path> batch_dir;` — `--batch` directory.
- L33 `std::optional<int> jobs;` — `--jobs` worker count (reserved).
- L34 `std::string voice = "en";` — `--voice` (default en).
- L35 `int speed = 175;` — `--speed` (default 175 wpm).
- L36 `bool speak = true;` — speak aloud unless `--no-speak`.
- L37 `bool null_tts = false;` — force the null engine.
- L38 `bool verbose = false;` — diagnostics flag.
- L39 `bool show_help = false;` — `--help` flag.
- L40 `};` — closes `Args`.
- L42 `void print_help(std::ostream& os, const char* prog) {` — prints usage.
- L43 `os << "spoken " << spoken::version_string << "\n"` — banner with version.
- L44-L57 (usage lines) — one `os <<` per option, describing each flag.
- L58 `}` — closes `print_help`.

### Argument parsing

- L60 `[[nodiscard]] std::optional<Args> parse_args(int argc, char** argv) {` — parses
  argv into `Args`; `nullopt` signals an error (exit 2).
- L61 `Args a;` — result being filled.
- L62 `for (int i = 1; i < argc; ++i) {` — iterate arguments (skip argv[0]).
- L63 `const std::string_view arg{argv[i]};` — current token as a view.
- L64 `auto next = [&](std::string_view name) -> std::optional<std::string> {` — helper
  to fetch the following value for an option.
- L65 `if (i + 1 >= argc) {` — missing value...
- L66 `std::cerr << "error: " << name << " needs an argument\n";` — report it...
- L67 `return std::nullopt;` — and signal failure.
- L68 `}` — ends the missing-value check.
- L69 `return std::string{argv[++i]};` — consume and return the value.
- L70 `};` — ends the helper.
- L72 `if (arg == "-h" || arg == "--help") {` — help flag...
- L73 `a.show_help = true;` — set it.
- L74-L75 `--file` — `if (auto v = next(arg)) a.file = *v; else return std::nullopt;` —
  parse value or fail (init-statement `if`).
- L76-L77 `--out` — same pattern for the output path.
- L78-L79 `--wav` — same pattern for the WAV path.
- L80-L81 `--jobs` — `a.jobs = std::stoi(*v)` parse worker count or fail.
- L82-L83 `--batch` — parse the batch directory or fail.
- L84-L85 `--voice` — parse the voice or fail.
- L86-L87 `--speed` — `std::stoi` the speed or fail.
- L88-L89 `--no-speak` — `a.speak = false;`.
- L90-L91 `--null-tts` — `a.null_tts = true;`.
- L92-L93 `--verbose` — `a.verbose = true;`.
- L94 `} else if (!arg.empty() && arg[0] == '-') {` — any other dash-prefixed token is an
  unknown flag (DEF-1: avoids C++20 `starts_with`).
- L95 `std::cerr << "error: unknown flag " << arg << '\n';` — report it...
- L96 `return std::nullopt;` — and fail.
- L97 `} else {` — otherwise it is positional text.
- L98 `a.text.emplace_back(arg);` — collect the word.
- L99 `}` — ends the flag dispatch.
- L100 `}` — ends the argument loop.
- L101 `return a;` — return the parsed options.
- L102 `}` — closes `parse_args`.

### IO helpers and RecordResult

- L104 `[[nodiscard]] std::optional<std::string> read_text_file(const fs::path& p) {` —
  reads a file fully, or `nullopt` on failure.
- L105 `std::ifstream in{p};` — open the file.
- L106 `if (!in) return std::nullopt;` — failure if it cannot open.
- L107 `std::ostringstream ss;` — buffer.
- L108 `ss << in.rdbuf();` — slurp the whole file.
- L109 `return ss.str();` — return the contents.
- L110 `}` — closes `read_text_file`.
- L112 `[[nodiscard]] bool write_text_file(const fs::path& p, std::string_view text) {` —
  writes text to a file; returns success.
- L113 `std::ofstream out{p};` — open for writing.
- L114 `if (!out) return false;` — failure if it cannot open.
- L115 `out << text;` — write the text.
- L116 `return out.good();` — report stream status.
- L117 `}` — closes `write_text_file`.
- L119 `struct RecordResult {` — outcome of processing one batch record.
- L120 `fs::path source;` — input file.
- L121 `fs::path written_to;` — output file written.
- L122 `std::string spoken_text;` — the converted sentence.
- L123 `bool ok = false;` — success flag.
- L124 `std::string error;` — error message if any.
- L125 `};` — closes `RecordResult`.
- L127 `}  // namespace` — closes the anonymous namespace.

### main: setup

- L129 `int main(int argc, char** argv) try {` — function-try-block entry point.
- L130 `auto args = parse_args(argc, argv);` — parse the command line.
- L131 `if (!args) return 2;` — bad arguments -> exit 2.
- L132 `if (args->show_help) { print_help(std::cout, argv[0]); return 0; }` — help -> exit
  0.
- L134 `spoken::tts::EngineConfig ec;` — engine config built field-by-field (DEF-8).
- L135 `ec.voice = args->voice;` — set voice.
- L136 `ec.speed_wpm = args->speed;` — set speed.
- L137 `ec.kind = args->null_tts ? spoken::tts::EngineKind::Null` — choose engine kind:
  null if forced...
- L138 `: spoken::tts::EngineKind::Auto;` — otherwise auto-detect.
- L139 `if (args->out && args->speak && !args->wav) {` — if text output + speak + no
  wav...
- L140 `ec.text_sink = args->out->string();` — route null-engine speech text to the
  output file.
- L141 `}` — ends the sink setup.
- L143 `std::unique_ptr<spoken::tts::TtsEngine> engine;` — owned engine.
- L144 `try {` — guard engine construction.
- L145 `engine = spoken::tts::make_engine(ec);` — build it.
- L146 `} catch (const spoken::TtsError& e) {` — engine init failed...
- L147 `std::cerr << "TTS init failed: " << e.what() << '\n';` — report...
- L148 `return 3;` — exit 3.
- L149 `}` — ends the try/catch.
- L150 `if (args->verbose) std::cerr << "engine: " << engine->name() << '\n';` — log the
  chosen engine.
- L152 `const spoken::ConversionPipeline pipe{};` — the (stateless) pipeline.

### main: batch branch

- L154 `if (args->batch_dir) {` — batch mode.
- L155 `const fs::path in_dir = *args->batch_dir;` — input directory.
- L156 `const fs::path out_dir = args->out.value_or(in_dir / "out");` — output dir
  (default `<in>/out`).
- L157 `const fs::path wav_dir = args->wav.value_or(fs::path{});` — optional WAV dir.
- L158 `const bool want_wav = args->wav.has_value();` — whether to emit WAVs.
- L160 `std::error_code ec2;` — non-throwing filesystem error holder.
- L161 `fs::create_directories(out_dir, ec2);` — ensure the output dir exists.
- L162 `if (want_wav) fs::create_directories(wav_dir, ec2);` — and the wav dir.
- L164 `std::vector<fs::path> files;` — collected input files.
- L165 `for (const auto& e : fs::directory_iterator(in_dir, ec2)) {` — iterate the dir.
- L166 `if (e.is_regular_file() && e.path().extension() == ".txt") {` — pick `.txt`
  files.
- L167 `files.push_back(e.path());` — collect them.
- L168 `}` / L169 `}` — close the if and the loop.
- L170 `std::sort(files.begin(), files.end());` — deterministic order by path (Q-5/S7,
  sequential sort).
- L172 `std::mutex engine_mtx;` — serializes engine calls across workers (DEF-10).
- L173 `std::atomic<std::size_t> done{0};` — progress counter (atomic).
- L174 `const auto t0 = std::chrono::steady_clock::now();` — start time.
- L176 `std::vector<std::future<RecordResult>> futs;` — futures for each file.
- L177 `futs.reserve(files.size());` — pre-size.
- L178 `for (const auto& f : files) {` — launch a task per file.
- L179 `futs.push_back(std::async(std::launch::async,` — force a real async task...
- L180 `[&, f]() -> RecordResult {` — task lambda capturing context by ref and the path
  by value.
- L181 `RecordResult r{f, {}, "", false, {}};` — initialize the result.
- L182 `auto txt = read_text_file(f);` — read the file (in parallel).
- L183 `if (!txt) { r.error = "cannot read"; return r; }` — read failure.
- L184 `try {` — guard conversion/IO.
- L185 `r.spoken_text = pipe.convert(*txt);` — convert text (thread-safe, const).
- L186 `const fs::path of = out_dir / f.filename();` — output path.
- L187 `if (!write_text_file(of, r.spoken_text)) {` — write the spoken text...
- L188 `r.error = "cannot write " + of.string();` — record a write error...
- L189 `return r;` — and stop this record.
- L190 `}` — ends the write check.
- L191 `r.written_to = of;` — record the output file.
- L192 `if (want_wav) {` — optional WAV synthesis.
- L193 `const fs::path wf =` — compute the wav path...
- L194 `wav_dir / (f.filename().string() + ".wav");` — `<wav_dir>/<name>.wav`.
- L195 `std::lock_guard<std::mutex> lk(engine_mtx);` — serialize the engine call
  (DEF-10).
- L196 `engine->to_wav(r.spoken_text, wf.string());` — synthesize under the lock.
- L197 `}` — ends the wav block.
- L198 `r.ok = true;` — mark success.
- L199 `} catch (const spoken::SpokenError& e) {` — capture library errors per record...
- L200 `r.error = e.what();` — store the message (don't abort the batch).
- L201 `}` — ends the try/catch.
- L202 `return r;` — return the record result.
- L203 `}));` — ends the lambda and the `std::async` push.
- L204 `}` — ends the launch loop.
- L206 `std::size_t ok = 0, fail = 0;` — tallies.
- L207 `for (auto& fut : futs) {` — collect results in order.
- L208 `auto r = fut.get();` — wait for and retrieve each result.
- L209 `++done;` — bump the progress counter.
- L210 `std::cout << "[" << done << '/' << futs.size() << "] "` — progress prefix...
- L211 `<< r.source.filename().string() << " -> "` — the source name...
- L212 `<< (r.ok ? r.written_to.string()` — the output path on success...
- L213 `: std::string{"FAILED: "} + r.error) << '\n';` — or the error.
- L214 `if (r.ok) ++ok; else ++fail;` — update tallies.
- L215 `}` — ends the collection loop.
- L216 `const auto dt = std::chrono::steady_clock::now() - t0;` — elapsed time.
- L217-L221 (summary `std::cout`) — prints ok/fail counts and elapsed milliseconds.
- L222 `return fail == 0 ? 0 : 1;` — exit 0 if all succeeded, else 1.
- L223 `}` — ends the batch branch.

### main: single-record branch

- L225 `std::string input;` — the single-input text.
- L226 `if (args->file) {` — read from `--file`...
- L227 `auto txt = read_text_file(*args->file);` — read it...
- L228 `if (!txt) { std::cerr << "cannot read " << *args->file << '\n'; return 1; }` —
  error -> exit 1.
- L229 `input = std::move(*txt);` — take the contents.
- L230 `} else if (!args->text.empty()) {` — else use positional words...
- L231 `for (std::size_t i = 0; i < args->text.size(); ++i) {` — join them...
- L232 `if (i) input += ' ';` — with single spaces...
- L233 `input += args->text[i];` — appending each word.
- L234 `}` — ends the join loop.
- L235 `} else {` — else read stdin...
- L236 `std::ostringstream ss;` — buffer...
- L237 `ss << std::cin.rdbuf();` — slurp stdin...
- L238 `input = ss.str();` — take it.
- L239 `}` — ends the input selection.
- L241 `const std::string spoken_text = pipe.convert(input);` — convert the input.
- L243 `if (args->wav) {` — WAV requested...
- L244 `engine->to_wav(spoken_text, args->wav->string());` — synthesize to file.
- L245 `} else if (args->out) {` — else if file output...
- L246 `if (!write_text_file(*args->out, spoken_text)) {` — write it...
- L247 `std::cerr << "cannot write " << *args->out << '\n';` — error message...
- L248 `return 1;` — exit 1 on failure.
- L249 `}` — ends the write check.
- L250 `} else {` — else print to stdout...
- L251 `std::cout << spoken_text << '\n';` — print the sentence.
- L252 `}` — ends the output selection.
- L254 `if (args->speak && !args->wav && engine->name() != "null") {` — speak aloud only
  with a real (non-null) engine and when not writing WAV.
- L255 `engine->speak(spoken_text);` — play the audio.
- L256 `}` — ends the speak block.
- L257 `return 0;` — success.
- L258 `}` — ends the main try-block body.

### main: exception handlers (exit-code mapping)

- L259 `catch (const spoken::SpokenError& e) {` — library errors...
- L260 `std::cerr << "spoken error: " << e.what() << '\n';` — report...
- L261 `return 1;` — exit 1.
- L262 `}` — ends handler.
- L263 `catch (const std::exception& e) {` — other standard exceptions...
- L264 `std::cerr << "fatal: " << e.what() << '\n';` — report...
- L265 `return 1;` — exit 1.
- L266 `}` — ends handler.
- L267 `catch (...) {` — any non-standard throw (S4)...
- L268 `std::cerr << "fatal: unexpected internal error\n";` — report...
- L269 `return 4;` — exit 4 (contract violation), so the process never terminates
  uncaught.
- L270 `}` — ends the catch-all and the function-try-block.

---

## Notes

- Line numbers are best-effort against the files as written; the quoted code makes each
  entry unambiguous even if a file is later edited (append-only per project rules).
- Test files (`tests/*.cpp`) and the build files (`CMakeLists.txt`) are intentionally
  out of scope here; request a follow-up file to document them line by line.
