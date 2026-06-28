#include "spoken/tts/null_engine.hpp"         // the class being defined
#include "spoken/error.hpp"                   // TtsError for failures

#include <fstream>                            // file output in to_wav
#include <ostream>                            // stream insertion
#include <utility>                            // std::move

namespace spoken::tts {                       // nested TTS namespace

// Store the sink (moved in) and reject an empty callback.
NullEngine::NullEngine(Sink on_text) : sink_{std::move(on_text)} {
    if (!sink_) throw TtsError{"NullEngine needs a non-empty sink"};  // empty std::function -> error
}

// "Speak" by forwarding the text to the configured sink.
void NullEngine::speak(std::string_view text) {
    sink_(text);                              // invoke the callback
}

// Placeholder WAV: write a human-readable line instead of audio.
void NullEngine::to_wav(std::string_view text, const std::string& wav_path) {
    std::ofstream out{wav_path};              // open the target file
    if (!out) throw TtsError{"cannot open " + wav_path};  // error if it cannot open
    out << "# spoken null TTS, audio would be: \"" << text << "\"\n";  // write the placeholder
}

}  // namespace spoken::tts                    // close the namespace
