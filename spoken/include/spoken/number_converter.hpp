#pragma once                                  // include-once guard

#include <array>                              // std::array lookup tables
#include <cstdint>                            // std::int64_t
#include <string>                             // return type
#include <string_view>                        // table element type and the lexeme parameter

namespace spoken {                            // library root namespace

// Pure, stateless integer/decimal-to-words converter.
class NumberConverter {
public:
    // Convert an integer to English words.
    [[nodiscard]] std::string to_words(std::int64_t value) const;

    // Convert a decimal from its original literal (Q-4/DEF-4: exact digits, no float artifacts).
    [[nodiscard]] std::string decimal_to_words(std::string_view lexeme) const;

private:
    // Words for 0..19 (constexpr static members are implicitly inline in C++17).
    static constexpr std::array<std::string_view, 20> kOnes = {
        "zero", "one", "two", "three", "four", "five", "six", "seven",
        "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
        "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
    };
    // Tens words; indices 0 and 1 are unused placeholders.
    static constexpr std::array<std::string_view, 10> kTens = {
        "", "", "twenty", "thirty", "forty", "fifty",
        "sixty", "seventy", "eighty", "ninety"
    };
    // Scale words by 3-digit group; index 0 unused.
    static constexpr std::array<std::string_view, 7> kIllions = {
        "", "thousand", "million", "billion", "trillion",
        "quadrillion", "quintillion"
    };
};

}  // namespace spoken                        // close the namespace
