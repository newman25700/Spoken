#pragma once                                  // include-once guard

#include "spoken/tokens.hpp"                  // the Token type this lexer produces

#include <string_view>                        // input is a non-owning std::string_view
#include <vector>                             // output is a std::vector<Token>

namespace spoken {                            // library root namespace

// Stateless lexer turning text into tokens.
class Tokenizer {
public:
    // Convert text to tokens; [[nodiscard]] warns if the result is ignored.
    [[nodiscard]] std::vector<Token> tokenize(std::string_view text) const;
};

// Free-function convenience wrapper (the tokenizer is stateless).
[[nodiscard]] inline std::vector<Token> tokenize(std::string_view text) {
    return Tokenizer{}.tokenize(text);        // construct a temporary and delegate
}

}  // namespace spoken                        // close the namespace
