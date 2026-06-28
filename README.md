# Spoken

A C++17 command-line tool and library that converts a line of mixed text
(words, numbers, symbols) into a natural spoken-English sentence — and can
speak it aloud or write a WAV via an offline TTS engine.

Quick example:
    spoken "I have 42 apples and 3.14 pies"
    -> I have forty-two apples and three point one four pies.

## Build & test (quick)
    cmake -S spoken -B spoken/build -DSPOKEN_BUILD_TESTS=ON
    cmake --build spoken/build -j
    ctest --test-dir spoken/build --output-on-failure

## Full documentation
See the complete README — features, build options (CMake / direct clang++ /
sanitizers), full CLI reference, library API, design notes, and the docs map —
in `document/readme.md`.

## License
