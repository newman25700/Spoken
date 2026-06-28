#include "spoken/number_converter.hpp"        // the class being defined

#include <algorithm>                          // std::remove in the decimal path
#include <cstdio>                             // retained for portability of formatting helpers
#include <string>                             // std::stoll and string building

namespace spoken {                            // library root namespace
namespace {                                   // file-local helper

// Render 0..999 as words using the ones/teens and tens tables.
std::string three_digit(std::uint64_t n,
                        const std::array<std::string_view, 20>& ones,   // 0..19 words
                        const std::array<std::string_view, 10>& tens) { // tens words
    std::string out;                          // accumulates the words
    if (n >= 100) {                           // hundreds present
        out += std::string{ones[n / 100]};    // leading digit word
        out += " hundred";                    // the word "hundred"
        n %= 100;                             // drop the hundreds digit
        if (n) out += ' ';                    // separator if a remainder follows
    }
    if (n >= 20) {                            // tens-and-ones (20..99)
        out += std::string{tens[n / 10]};     // tens word
        if (n % 10) { out += '-'; out += std::string{ones[n % 10]}; }  // hyphen + ones (forty-two)
    } else if (n > 0) {                        // 1..19
        out += std::string{ones[n]};          // direct lookup
    }
    return out;                               // 0..999 rendering
}

}  // namespace                               // close the anonymous namespace

// Convert a signed integer to words.
std::string NumberConverter::to_words(std::int64_t value) const {
    if (value == 0) return std::string{kOnes[0]};  // fast path: "zero"

    const bool negative = value < 0;          // remember the sign
    std::uint64_t n = negative                // compute the magnitude as unsigned...
        ? (~static_cast<std::uint64_t>(value) + 1ULL)  // two's-complement negate (safe for INT64_MIN)
        : static_cast<std::uint64_t>(value);  // non-negative case

    std::string magnitude;                    // built separately so "negative" is not lost (DEF-3)
    int chunk_idx = 0;                         // index into the scale words
    bool any = false;                          // whether any chunk has been emitted
    while (n > 0 && chunk_idx < static_cast<int>(kIllions.size())) {  // process 3-digit groups
        std::uint64_t chunk = n % 1000;       // lowest three digits
        if (chunk != 0) {                     // skip empty groups
            std::string part = three_digit(chunk, kOnes, kTens);  // words for this group
            if (chunk_idx > 0) {              // append scale word for higher groups
                part += ' ';                  // separator
                part += std::string{kIllions[chunk_idx]};  // "thousand"/"million"/...
            }
            if (any) { part += ' '; part += magnitude; }  // prepend this group before the lower part
            magnitude = std::move(part);      // adopt the combined string
            any = true;                       // output now exists
        }
        n /= 1000;                            // next group
        ++chunk_idx;                          // advance scale index
    }

    return negative ? ("negative " + magnitude) : magnitude;  // prepend sign word once (DEF-3)
}

// Convert a decimal from its original literal (digit-accurate).
std::string NumberConverter::decimal_to_words(std::string_view lexeme) const {
    if (lexeme.empty()) return std::string{kOnes[0]};  // empty -> "zero"

    bool negative = false;                    // sign flag
    std::size_t start = 0;                    // index where digits begin
    if (lexeme[0] == '-') { negative = true; start = 1; }  // handle a leading minus

    const std::size_t dot = lexeme.find('.', start);  // locate the decimal point
    std::string int_digits{lexeme.substr(     // extract integer-part digits...
        start, dot == std::string_view::npos ? std::string_view::npos  // to end if no dot...
                                             : dot - start)};           // else up to the dot
    int_digits.erase(std::remove(int_digits.begin(), int_digits.end(), ','),  // strip thousands commas
                     int_digits.end());
    if (int_digits.empty()) int_digits = "0"; // treat ".5" as "0.5"

    std::string out;                          // output accumulator
    if (negative) out += "negative ";         // sign word first
    out += to_words(static_cast<std::int64_t>(std::stoll(int_digits)));  // speak the integer part

    if (dot != std::string_view::npos) {      // if a fractional part exists...
        out += " point";                      // the word "point"
        for (std::size_t k = dot + 1; k < lexeme.size(); ++k) {  // iterate fractional characters
            const char d = lexeme[k];         // current character
            if (d >= '0' && d <= '9') {       // speak only real digits (DEF-4: no zero padding)
                out += ' ';                   // separator
                out += std::string{kOnes[static_cast<std::size_t>(d - '0')]};  // the digit word
            }
        }
    }
    return out;                               // the spoken decimal
}

}  // namespace spoken                        // close the namespace
