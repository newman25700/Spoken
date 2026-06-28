cmake_minimum_required(VERSION 3.16)
project(spoken
    VERSION     1.0.0
    DESCRIPTION "Convert text to spoken English + offline TTS"
    LANGUAGES   CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SPOKEN_BUILD_TESTS        "Build unit tests"          ON)
option(SPOKEN_BUILD_EXAMPLES     "Build example programs"    OFF)
option(SPOKEN_WARNINGS_AS_ERRORS "Treat warnings as errors"  OFF)
option(SPOKEN_WITH_ESPEAK        "Use eSpeak-ng for real TTS" ON)
set(SPOKEN_SANITIZERS "" CACHE STRING
    "Comma list: address,undefined,thread,memory")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic -Wshadow -Wconversion)
elseif(MSVC)
    add_compile_options(/W4 /permissive-)
endif()
if(SPOKEN_WARNINGS_AS_ERRORS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Werror)
    else()
        add_compile_options(/WX)
    endif()
endif()

if(SPOKEN_SANITIZERS)
    if("address" IN_LIST SPOKEN_SANITIZERS OR "undefined" IN_LIST SPOKEN_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
    if("thread" IN_LIST SPOKEN_SANITIZERS)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
endif()

add_library(spoken
    src/tokens.cpp
    src/tokenizer.cpp
    src/number_converter.cpp
    src/symbol_converter.cpp
    src/sentence_builder.cpp
    src/conversion_pipeline.cpp
    src/tts/null_engine.cpp
    src/tts/factory.cpp
)
target_include_directories(spoken PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(spoken PUBLIC cxx_std_17)

if(SPOKEN_WITH_ESPEAK)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(ESPEAK IMPORTED_TARGET espeak-ng)
    endif()
    if(ESPEAK_FOUND)
        message(STATUS "eSpeak-ng found; enabling real TTS engine")
        target_sources(spoken PRIVATE src/tts/espeak_engine.cpp)
        target_link_libraries(spoken PUBLIC PkgConfig::ESPEAK)
        target_compile_definitions(spoken PUBLIC SPOKEN_HAVE_ESPEAK=1)
    else()
        message(WARNING "eSpeak-ng not found; building null-TTS only")
        target_compile_definitions(spoken PUBLIC SPOKEN_HAVE_ESPEAK=0)
    endif()
else()
    target_compile_definitions(spoken PUBLIC SPOKEN_HAVE_ESPEAK=0)
endif()

add_executable(spoken-cli apps/spoken.cpp)
target_link_libraries(spoken-cli PRIVATE spoken)

if(SPOKEN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
