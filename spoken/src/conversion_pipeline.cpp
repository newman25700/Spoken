#include "spoken/conversion_pipeline.hpp"     // the class being defined

#include <algorithm>                          // std::transform
#include <iterator>                           // std::back_inserter
#include <string>                             // string handling
#include <type_traits>                        // std::decay_t / std::is_same_v in the visitor
#include <vector>                             // intermediate vector of forms

namespace spoken {                            // library root namespace

ConversionPipeline::ConversionPipeline() = default;  // members default-construct (Rule of 0)

namespace {                                   // file-local helper

// Convert one token to its spoken form via a type-dispatched visitor.
std::string token_to_spoken(const Token& t,
                            const NumberConverter& nc,   // number converter
                            const SymbolConverter& sc) { // symbol converter
    return std::visit([&](const auto& v) -> std::string {  // visit the active alternative
        using T = std::decay_t<decltype(v)>;  // the alternative's concrete type
        if constexpr (std::is_same_v<T, Word>) {              // word...
            return v.text;                                    // speak as-is
        } else if constexpr (std::is_same_v<T, Integer>) {    // integer...
            return nc.to_words(v.value);                      // via integer renderer
        } else if constexpr (std::is_same_v<T, Decimal>) {    // decimal...
            return nc.decimal_to_words(v.lexeme);             // render from original lexeme (Q-4)
        } else if constexpr (std::is_same_v<T, Symbol>) {     // symbol...
            return sc.to_words(v.glyph);                      // map glyph to words
        } else if constexpr (std::is_same_v<T, Punctuation>) {// punctuation...
            return std::string{v.mark};                       // pass the mark through
        } else if constexpr (std::is_same_v<T, Whitespace>) { // whitespace...
            return std::string{};                             // drop it (Q-1); builder owns spacing
        }
    }, t);                                    // apply the visitor to token t
}

}  // namespace                               // close the anonymous namespace

// End-to-end: tokenize, map each token to words, then assemble the sentence.
std::string ConversionPipeline::convert(std::string_view text) const {
    const auto tokens = spoken::tokenize(text);  // step 1: tokenize

    std::vector<std::string> spoken_per_token;   // per-token spoken forms
    spoken_per_token.reserve(tokens.size());     // pre-size to avoid reallocation
    std::transform(tokens.begin(), tokens.end(), // step 2: map each token...
                   std::back_inserter(spoken_per_token),  // appending results...
                   [&](const Token& t) {         // via a lambda capturing the converters
                       return token_to_spoken(t, numbers_, symbols_);
                   });

    return builder_.assemble(spoken_per_token);  // step 3: assemble the sentence
}

}  // namespace spoken                        // close the namespace
