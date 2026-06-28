#pragma once                                  // include-once guard

#include <string>                             // return type and map value
#include <string_view>                        // query parameter type
#include <unordered_map>                      // the glyph-to-words table

namespace spoken {                            // library root namespace

// Maps symbols/operators to spoken words, with a spell-out fallback.
class SymbolConverter {
public:
    SymbolConverter();                        // constructor builds the lookup table

    // Convert a glyph to words (falls back to spelling unknown glyphs).
    [[nodiscard]] std::string to_words(std::string_view glyph) const;
    // Report whether a glyph is in the table; cannot throw.
    [[nodiscard]] bool        known(std::string_view glyph) const noexcept;

private:
    std::unordered_map<std::string, std::string> table_;  // glyph table (Rule of 0: self-managing)
};

}  // namespace spoken                        // close the namespace
