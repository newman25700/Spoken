#include "spoken/tokenizer.hpp"               // the class being defined
#include "spoken/error.hpp"                   // InvalidInputError for bad numbers

#include <algorithm>                          // std::remove for stripping separators
#include <cctype>                             // character classification (isalpha, etc.)
#include <string>                             // string building and std::stoll/std::stod

namespace spoken {                            // library root namespace
namespace {                                   // file-local helpers (internal linkage)

bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }  // ASCII digit test (locale-free)
bool is_alpha(char c) noexcept {                                 // letter test...
    return std::isalpha(static_cast<unsigned char>(c)) != 0;     // cast avoids UB on negative chars
}
bool is_space(char c) noexcept {                                 // whitespace test...
    return std::isspace(static_cast<unsigned char>(c)) != 0;
}
bool is_punct(char c) noexcept {                                 // punctuation test...
    return std::ispunct(static_cast<unsigned char>(c)) != 0;
}

}  // namespace                               // close the anonymous namespace

// Scan the input left-to-right, emitting one token per lexical run.
std::vector<Token> Tokenizer::tokenize(std::string_view text) const {
    std::vector<Token> out;                   // accumulates produced tokens
    out.reserve(text.size() / 4 + 1);         // rough pre-size to limit reallocations

    std::size_t i = 0;                        // current scan index
    while (i < text.size()) {                 // until the input is consumed
        const char c = text[i];               // current character

        if (is_space(c)) {                    // --- whitespace run ---
            std::size_t j = i;                // start of the run
            while (j < text.size() && is_space(text[j])) ++j;  // extend to run end
            out.emplace_back(Whitespace{std::string{text.substr(i, j - i)}});  // emit the run
            i = j;                            // advance past it
        } else if (is_digit(c)) {             // --- number ---
            std::size_t j = i;                // start of the number
            bool seen_dot = false;            // tracks a decimal point
            while (j < text.size() &&         // continue while the char is part of a number...
                   (is_digit(text[j]) || text[j] == '.' || text[j] == ',')) {  // digit, dot, or comma
                if (text[j] == '.') seen_dot = true;  // note a decimal point
                ++j;                          // advance
            }
            std::string lexeme{text.substr(i, j - i)};  // keep the original literal (Q-4)
            std::string num = lexeme;         // working copy to normalize
            num.erase(std::remove(num.begin(), num.end(), ','), num.end());  // strip thousands commas
            try {                             // guard numeric parsing
                if (seen_dot) {               // decimal...
                    out.emplace_back(Decimal{std::stod(num), lexeme});  // value + original lexeme
                } else {                      // integer...
                    out.emplace_back(Integer{std::stoll(num), lexeme}); // value + original lexeme
                }
            } catch (const std::exception&) { // parse failure (e.g. overflow)
                throw InvalidInputError{"cannot parse number: " + num};  // rethrow as typed error
            }
            i = j;                            // advance past the number
        } else if (is_alpha(c)) {             // --- word ---
            std::size_t j = i;                // start of the word
            while (j < text.size() &&         // continue while word characters follow...
                   (is_alpha(text[j]) || text[j] == '\'' || text[j] == '-')) {  // letters, ' and -
                ++j;                          // advance
            }
            out.emplace_back(Word{std::string{text.substr(i, j - i)}});  // emit the word
            i = j;                            // advance
        } else if (is_punct(c)) {             // --- punctuation / symbol ---
            std::size_t j = i + 1;            // tentatively take one char
            if (j < text.size() && is_punct(text[j])) ++j;  // grab a 2nd for operators like "!="
            std::string glyph{text.substr(i, j - i)};       // the captured 1-2 char glyph
            if (glyph.size() == 1 &&                         // a single char...
                std::string{".!,?;:"}.find(c) != std::string::npos) {  // that is sentence punctuation
                out.emplace_back(Punctuation{c});           // emit a Punctuation token
            } else {                          // otherwise it's a symbol/operator
                out.emplace_back(Symbol{std::move(glyph)});  // emit a Symbol (move the string)
            }
            i = j;                            // advance past the glyph
        } else {                              // --- any other byte ---
            out.emplace_back(Symbol{std::string(1, c)});  // emit it as a single-char Symbol
            ++i;                              // advance one byte
        }
    }
    return out;                               // return the token list
}

}  // namespace spoken                        // close the namespace
