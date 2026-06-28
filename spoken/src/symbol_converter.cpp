#include "spoken/symbol_converter.hpp"        // the class being defined

namespace spoken {                            // library root namespace

// Constructor: populate the glyph -> words table via its initializer list.
SymbolConverter::SymbolConverter()
    : table_{
        {"$",  "dollars"},                    // currency
        {"%",  "percent"},                    // percent sign
        {"@",  "at"},                         // at sign
        {"&",  "and"},                        // ampersand
        {"+",  "plus"},                       // plus
        {"-",  "minus"},                      // minus / hyphen
        {"*",  "times"},                      // asterisk / multiply
        {"/",  "divided by"},                 // slash / divide
        {"=",  "equals"},                     // equals
        {"!=", "not equals"},                 // inequality operator
        {"==", "equals"},                     // equality operator
        {"<",  "less than"},                  // less-than
        {">",  "greater than"},               // greater-than
        {"<=", "less than or equal to"},      // <=
        {">=", "greater than or equal to"},   // >=
        {"->", "arrow"},                      // arrow operator
        {"=>", "arrow"},                      // fat arrow
    } {}                                       // empty constructor body

// Look up a glyph; if unknown, spell out its characters.
std::string SymbolConverter::to_words(std::string_view glyph) const {
    auto it = table_.find(std::string{glyph});         // search the table
    if (it != table_.end()) return it->second;         // return the known word

    std::string out;                          // fallback accumulator
    for (char c : glyph) {                     // iterate each character
        if (!out.empty()) out += ' ';         // space between spelled characters
        switch (c) {                          // name common special characters
            case '#':  out += "hash";      break;
            case '~':  out += "tilde";     break;
            case '`':  out += "backtick";  break;
            case '^':  out += "caret";     break;
            case '|':  out += "pipe";      break;
            case '\\': out += "backslash"; break;
            default:   out += c;           break;  // otherwise keep the character itself
        }
    }
    return out;                               // the spelled-out fallback
}

// Membership test: true iff the glyph is in the table.
bool SymbolConverter::known(std::string_view glyph) const noexcept {
    return table_.find(std::string{glyph}) != table_.end();
}

}  // namespace spoken                        // close the namespace
