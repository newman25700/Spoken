add_executable(spoken_tests
    test_main.cpp
    test_tokenizer.cpp
    test_number_converter.cpp
    test_symbol_converter.cpp
    test_sentence_builder.cpp
    test_pipeline_e2e.cpp
)
target_link_libraries(spoken_tests PRIVATE spoken)
target_include_directories(spoken_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME spoken_tests COMMAND spoken_tests)
