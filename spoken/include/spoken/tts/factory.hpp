#pragma once                                  // include-once guard

#include <memory>                             // std::unique_ptr return type
#include <string>                             // config string fields

#include "spoken/tts/null_engine.hpp"         // a possible product
#include "spoken/tts/tts_engine.hpp"          // the returned interface type

#if defined(SPOKEN_HAVE_ESPEAK) && SPOKEN_HAVE_ESPEAK   // only when the build enabled eSpeak
#include "spoken/tts/espeak_engine.hpp"       // real engine header (guarded)
#endif

namespace spoken::tts {                       // nested TTS namespace

enum class EngineKind { Auto, Espeak, Null }; // scoped enum of engine choices

// Configuration passed to the factory.
struct EngineConfig {
    EngineKind  kind      = EngineKind::Auto; // which engine (Auto resolves at runtime)
    std::string voice     = "en";             // requested voice
    int         speed_wpm = 175;              // speech rate (words per minute)
    std::string text_sink;                    // optional file path for null-engine text
};

// Factory: returns an owned engine via the base interface.
[[nodiscard]] std::unique_ptr<TtsEngine> make_engine(const EngineConfig& cfg);

}  // namespace spoken::tts                    // close the namespace
