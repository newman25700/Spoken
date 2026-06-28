#include "test_framework.hpp"
#include "spoken/number_converter.hpp"

using spoken::NumberConverter;

TEST(small_integers) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(0),   std::string{"zero"});
    REQUIRE_EQ(nc.to_words(1),   std::string{"one"});
    REQUIRE_EQ(nc.to_words(13),  std::string{"thirteen"});
    REQUIRE_EQ(nc.to_words(21),  std::string{"twenty-one"});
    REQUIRE_EQ(nc.to_words(100), std::string{"one hundred"});
    REQUIRE_EQ(nc.to_words(115), std::string{"one hundred fifteen"});
    REQUIRE_EQ(nc.to_words(123), std::string{"one hundred twenty-three"});
}

TEST(negatives_keep_prefix) {            // DEF-3
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(-5), std::string{"negative five"});
    REQUIRE_EQ(nc.to_words(-1234),
               std::string{"negative one thousand two hundred thirty-four"});
}

TEST(thousands_and_millions) {
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(1000),    std::string{"one thousand"});
    REQUIRE_EQ(nc.to_words(1000000), std::string{"one million"});
    REQUIRE_EQ(nc.to_words(2000005), std::string{"two million five"});
}

TEST(decimals_from_lexeme) {             // DEF-4
    NumberConverter nc;
    REQUIRE_EQ(nc.decimal_to_words("3.14"), std::string{"three point one four"});
    REQUIRE_EQ(nc.decimal_to_words("0.5"),  std::string{"zero point five"});
    REQUIRE_EQ(nc.decimal_to_words("-2.71"),
               std::string{"negative two point seven one"});
}

TEST(runtime_zero) {                     // DEF-11: runtime, not static_assert
    NumberConverter nc;
    REQUIRE_EQ(nc.to_words(0), std::string{"zero"});
}
