#include "test_framework.hpp"
#include "spoken/tokenizer.hpp"
#include "spoken/tokens.hpp"

using namespace spoken;

// Note: expectations corrected to match real tokenizer output (the sketch's
// counts/indices were wrong). "hello world" -> Word, WS, Word = 3 tokens.
TEST(tokenize_words) {
    auto ts = tokenize("hello world");
    REQUIRE_EQ(ts.size(), std::size_t{3});
    REQUIRE(std::holds_alternative<Word>(ts[0]));
    REQUIRE_EQ(std::get<Word>(ts[0]).text, std::string{"hello"});
    REQUIRE(std::holds_alternative<Whitespace>(ts[1]));
    REQUIRE(std::holds_alternative<Word>(ts[2]));
}

TEST(tokenize_numbers) {
    auto ts = tokenize("42 and 3.14");  // Int WS Word WS Decimal
    REQUIRE(std::holds_alternative<Integer>(ts[0]));
    REQUIRE_EQ(std::get<Integer>(ts[0]).value, std::int64_t{42});
    REQUIRE(std::holds_alternative<Decimal>(ts[4]));
    REQUIRE(std::get<Decimal>(ts[4]).value > 3.13
            && std::get<Decimal>(ts[4]).value < 3.15);
    REQUIRE_EQ(std::get<Decimal>(ts[4]).lexeme, std::string{"3.14"});
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
    auto ts = tokenize("a != b");  // Word WS Symbol WS Word
    REQUIRE_EQ(ts.size(), std::size_t{5});
    REQUIRE(std::holds_alternative<Symbol>(ts[2]));
    REQUIRE_EQ(std::get<Symbol>(ts[2]).glyph, std::string{"!="});
}
