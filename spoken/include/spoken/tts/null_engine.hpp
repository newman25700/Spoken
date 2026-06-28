#pragma once                                  // include-once guard

#include "spoken/tts/tts_engine.hpp"          // base interface implemented here

#include <functional>                         // std::function sink type
#include <string>                             // name()/path types
#include <string_view>                        // text parameters

namespace spoken::tts {                       // nested TTS namespace

// A TtsEngine that produces no audio; it dispatches spoken text to a sink.
// Used by tests, headless environments, and --no-speak/--null-tts mode.
class NullEngine final : public TtsEngine {   // 'final': no further derivation
public:
    using Sink = std::function<void(std::string_view)>;  // text-receiving callback type

    explicit NullEngine(Sink on_text);        // 'explicit' prevents implicit conversion

    void speak(std::string_view text) override;                           // forwards text to the sink
    void to_wav(std::string_view text, const std::string& wav_path) override;  // writes a placeholder file
    [[nodiscard]] std::string name() const override { return "null"; }    // engine id "null"

private:
    Sink sink_;                               // the stored callback
};

}  // namespace spoken::tts                    // close the namespace
