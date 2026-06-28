#include "test_framework.hpp"
#include "spoken/conversion_pipeline.hpp"

using spoken::ConversionPipeline;

TEST(pipeline_basic) {
    ConversionPipeline pipe;
    REQUIRE_EQ(pipe.convert("I have 42 apples."),
               std::string{"I have forty-two apples."});
}

TEST(pipeline_symbols) {
    ConversionPipeline pipe;
    REQUIRE_EQ(pipe.convert("50% of 200 is 100"),
               std::string{"Fifty percent of two hundred is one hundred."});
}

TEST(pipeline_decimal) {                   // DEF-4 end-to-end
    ConversionPipeline pipe;
    REQUIRE_EQ(pipe.convert("pi is 3.14"),
               std::string{"Pi is three point one four."});
}

// pipeline_currency removed (Q-2): currency fusion is Extension Exercise #2.
