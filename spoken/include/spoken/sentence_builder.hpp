#pragma once                                  // include-once guard

#include <string>                             // return type
#include <vector>                             // input is a vector of spoken forms

namespace spoken {                            // library root namespace

// Joins per-token spoken forms into one well-formed sentence.
class SentenceBuilder {
public:
    // Assemble the forms (taken by const reference, no copy) into a sentence.
    [[nodiscard]] std::string assemble(
        const std::vector<std::string>& spoken_per_token) const;
};

}  // namespace spoken                        // close the namespace
