#pragma once                                  // include-once guard

#include <stdexcept>                          // std::runtime_error / std::exception base classes
#include <string>                             // std::string used by inherited message constructors

namespace spoken {                            // library root namespace

// Root of the library's exception hierarchy; catch this to catch any library error.
class SpokenError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;  // inherit base constructors (SpokenError{"msg"})
};

// Thrown for malformed input (e.g. an unparseable number).
class InvalidInputError : public SpokenError {
public:
    using SpokenError::SpokenError;           // inherit the message constructor
};

// Thrown by TTS engines / the factory on failure.
class TtsError : public SpokenError {
public:
    using SpokenError::SpokenError;           // inherit the message constructor
};

}  // namespace spoken                        // close the namespace
