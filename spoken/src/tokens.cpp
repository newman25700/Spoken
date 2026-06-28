#include "spoken/tokens.hpp"                  // Token type and operator<< declaration

#include <ostream>                            // full std::ostream definition for <<
#include <type_traits>                        // std::decay_t / std::is_same_v for the visitor

namespace spoken {                            // library root namespace

// Stream insertion: prints a labelled form of whichever alternative is active.
std::ostream& operator<<(std::ostream& os, const Token& t) {
    std::visit([&os](const auto& v) {         // visit the active alternative (stream captured by ref)
        using T = std::decay_t<decltype(v)>;  // concrete alternative type (no ref/const)
        if constexpr (std::is_same_v<T, Word>) {              // Word branch
            os << "Word(" << v.text << ')';                   // print the word text
        } else if constexpr (std::is_same_v<T, Integer>) {    // Integer branch
            os << "Integer(" << v.value << ')';               // print the integer value
        } else if constexpr (std::is_same_v<T, Decimal>) {    // Decimal branch
            os << "Decimal(" << v.value << ')';               // print the decimal value
        } else if constexpr (std::is_same_v<T, Symbol>) {     // Symbol branch
            os << "Symbol(" << v.glyph << ')';                // print the glyph
        } else if constexpr (std::is_same_v<T, Punctuation>) {// Punctuation branch
            os << "Punct(" << v.mark << ')';                  // print the mark
        } else if constexpr (std::is_same_v<T, Whitespace>) { // Whitespace branch
            os << "WS(\"" << v.spaces << "\")";               // print the whitespace, quoted
        }
    }, t);                                    // apply the visitor to token t
    return os;                                // return the stream for chaining
}

}  // namespace spoken                        // close the namespace
