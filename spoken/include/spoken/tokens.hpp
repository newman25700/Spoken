#pragma once                                  // include-once guard

#include <cstdint>                            // std::int64_t for the integer token
#include <iosfwd>                             // forward declarations of stream types for operator<<
#include <string>                             // std::string token fields
#include <variant>                            // std::variant backing the Token sum type

namespace spoken {                            // library root namespace

struct Word        { std::string  text;   };                    // a word's text
struct Integer     { std::int64_t value;  std::string lexeme; }; // value + original digits (Q-4)
struct Decimal     { double       value;  std::string lexeme; }; // value + original literal (Q-4/DEF-4)
struct Symbol      { std::string  glyph;  };                    // a 1-2 char symbol/operator
struct Punctuation { char         mark;   };                    // a single punctuation mark
struct Whitespace  { std::string  spaces; };                    // a run of whitespace (dropped later, Q-1)

// Type-safe sum type: holds exactly one alternative at a time.
using Token =
    std::variant<Word, Integer, Decimal, Symbol, Punctuation, Whitespace>;

// Stream insertion for diagnostics; defined in tokens.cpp.
std::ostream& operator<<(std::ostream& os, const Token& t);

}  // namespace spoken                        // close the namespace
