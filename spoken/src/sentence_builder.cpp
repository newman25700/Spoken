#include "spoken/sentence_builder.hpp"        // the class being defined

#include <algorithm>                          // std::find_if for trimming
#include <cctype>                             // std::isspace / std::toupper
#include <string>                             // string building

namespace spoken {                            // library root namespace
namespace {                                   // file-local helpers

// True if a form is empty or made entirely of whitespace.
bool is_blank(const std::string& s) {
    for (char c : s) {                        // scan characters
        if (!std::isspace(static_cast<unsigned char>(c))) return false;  // any non-space -> not blank
    }
    return true;                              // all whitespace (or empty)
}

// True if the form is a single sentence-punctuation character.
bool is_punct_form(const std::string& s) {
    if (s.size() != 1) return false;          // must be exactly one char
    return std::string{".,!?;:"}.find(s[0]) != std::string::npos;  // and a known mark
}

}  // namespace                               // close the anonymous namespace

// Join forms into a sentence: punctuation attaches with no leading space (DEF-2).
std::string SentenceBuilder::assemble(
    const std::vector<std::string>& spoken_per_token) const {

    std::string out;                          // output sentence accumulator
    for (const auto& form : spoken_per_token) {  // iterate each spoken form
        if (form.empty() || is_blank(form)) continue;  // drop empty/whitespace forms (Q-1)
        if (is_punct_form(form)) {            // punctuation mark...
            out += form;                      // attach with no leading space (DEF-2)
        } else {                              // word/number/symbol...
            if (!out.empty()) out += ' ';     // single-space separator between tokens
            out += form;                      // append the form
        }
    }

    auto not_ws = [](char c) {                // predicate: a non-whitespace character
        return !std::isspace(static_cast<unsigned char>(c));
    };
    out.erase(out.begin(), std::find_if(out.begin(), out.end(), not_ws));  // trim leading whitespace
    out.erase(std::find_if(out.rbegin(), out.rend(), not_ws).base(), out.end());  // trim trailing

    if (!out.empty()) {                       // finalize only non-empty sentences
        out[0] = static_cast<char>(           // capitalize the first character...
            std::toupper(static_cast<unsigned char>(out[0])));  // via toupper with a safe cast
        const char last = out.back();         // inspect the final character
        if (last != '.' && last != '?' && last != '!') out += '.';  // ensure terminal punctuation
    }
    return out;                               // the assembled sentence
}

}  // namespace spoken                        // close the namespace
