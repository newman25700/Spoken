#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace test {

struct AssertionFailed : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Case {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<Case>& registry() {
    static std::vector<Case> cases;
    return cases;
}

struct Registrar {
    Registrar(std::string name, std::function<void()> fn) {
        registry().push_back({std::move(name), std::move(fn)});
    }
};

template <class... Bools>
constexpr bool all_true(Bools... bs) {
    return (bs && ... && true);
}

inline int run_all() {
    int passed = 0, failed = 0;
    for (const auto& c : registry()) {
        try {
            c.fn();
            std::cout << "[ PASS ] " << c.name << '\n';
            ++passed;
        } catch (const AssertionFailed& e) {
            std::cout << "[ FAIL ] " << c.name << "  -- " << e.what() << '\n';
            ++failed;
        } catch (const std::exception& e) {
            std::cout << "[ ERR  ] " << c.name << "  -- " << e.what() << '\n';
            ++failed;
        }
    }
    std::cout << "\n=== " << passed << " passed, " << failed << " failed ("
              << registry().size() << " total) ===\n";
    return failed == 0 ? 0 : 1;
}

}  // namespace test

#define TEST(name)                                                  \
    static void test_##name();                                      \
    static ::test::Registrar reg_##name(#name, &test_##name);       \
    static void test_##name()

#define REQUIRE(cond)                                               \
    do {                                                            \
        if (!(cond)) {                                              \
            std::ostringstream _oss;                                \
            _oss << __FILE__ << ":" << __LINE__                     \
                 << " REQUIRE failed: " #cond;                      \
            throw ::test::AssertionFailed{_oss.str()};             \
        }                                                           \
    } while (0)

#define REQUIRE_EQ(a, b)                                            \
    do {                                                            \
        auto _a = (a);                                              \
        auto _b = (b);                                              \
        if (!(_a == _b)) {                                          \
            std::ostringstream _oss;                                \
            _oss << __FILE__ << ":" << __LINE__                     \
                 << " REQUIRE_EQ failed: " #a " == " #b;            \
            throw ::test::AssertionFailed{_oss.str()};             \
        }                                                           \
    } while (0)
