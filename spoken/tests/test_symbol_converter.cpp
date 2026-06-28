#include "test_framework.hpp"
#include "spoken/symbol_converter.hpp"

using spoken::SymbolConverter;

TEST(symbol_lookup) {
    SymbolConverter sc;
    REQUIRE_EQ(sc.to_words("$"),  std::string{"dollars"});
    REQUIRE_EQ(sc.to_words("%"),  std::string{"percent"});
    REQUIRE_EQ(sc.to_words("=="), std::string{"equals"});
    REQUIRE_EQ(sc.to_words("->"), std::string{"arrow"});
    REQUIRE(sc.known("@"));
    REQUIRE(!sc.known("?!"));
}

TEST(symbol_fallback) {
    SymbolConverter sc;
    REQUIRE_EQ(sc.to_words("#"), std::string{"hash"});
}
