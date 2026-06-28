#pragma once                                  // include-once guard

#include "spoken/number_converter.hpp"        // owns a NumberConverter
#include "spoken/sentence_builder.hpp"        // owns a SentenceBuilder
#include "spoken/symbol_converter.hpp"        // owns a SymbolConverter
#include "spoken/tokenizer.hpp"               // uses the tokenizer

#include <string>                             // return type
#include <string_view>                        // input type

namespace spoken {                            // library root namespace

// Orchestrates tokenize -> convert-each-token -> assemble.
class ConversionPipeline {
public:
    ConversionPipeline();                     // constructor (defaulted in the .cpp)

    // End-to-end conversion entry point.
    [[nodiscard]] std::string convert(std::string_view text) const;

private:
    NumberConverter numbers_;                 // number-to-words converter (composition, Rule of 0)
    SymbolConverter symbols_;                 // symbol-to-words converter
    SentenceBuilder builder_;                 // final sentence assembler
};

}  // namespace spoken                        // close the namespace
