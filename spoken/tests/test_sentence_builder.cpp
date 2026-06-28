#include "test_framework.hpp"
#include "spoken/sentence_builder.hpp"

using spoken::SentenceBuilder;

TEST(capitalize_and_period) {
    SentenceBuilder sb;
    REQUIRE_EQ(sb.assemble({"hello", " ", "world"}),
               std::string{"Hello world."});
}

TEST(collapse_whitespace) {
    SentenceBuilder sb;
    REQUIRE_EQ(sb.assemble({"a", "  \t  ", "b"}), std::string{"A b."});
}

TEST(preserve_terminal_punctuation) {
    SentenceBuilder sb;
    REQUIRE_EQ(sb.assemble({"really?"}), std::string{"Really?"});
}

TEST(no_space_before_punctuation) {        // DEF-2
    SentenceBuilder sb;
    REQUIRE_EQ(sb.assemble({"apples", "."}), std::string{"Apples."});
}
