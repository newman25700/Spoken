#pragma once                                  // include-once guard

#include <string>                             // name() return and to_wav path
#include <string_view>                        // text parameters

namespace spoken::tts {                       // nested TTS namespace

// Abstract TTS interface (Strategy / Dependency-Inversion hook).
// Contract: implementations may throw spoken::TtsError on failure.
class TtsEngine {
public:
    virtual ~TtsEngine() = default;           // virtual dtor: safe deletion via base pointer

    virtual void speak(std::string_view text) = 0;                          // play text aloud
    virtual void to_wav(std::string_view text, const std::string& wav_path) = 0;  // synthesize to WAV
    [[nodiscard]] virtual std::string name() const = 0;                     // engine identifier
};

}  // namespace spoken::tts                    // close the namespace
