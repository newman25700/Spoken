
# The Complete C++17 Reference — A Tiered Learning Path

> **Status:** 🚧 Structure / Table of Contents only. No tier content written yet (per workflow step 1 — 
> awaiting approval before writing Tier 1).

---

## Role & Scope

This document is an authoritative, single-file Markdown reference for the **C++17** standard — covering the language core, 
object-oriented programming, and the Standard Template Library. It is organized as a strict **5-tier difficulty ladder**: 
each tier builds on the previous one, and no concept jumps ahead of its prerequisites.

Any feature requiring C++20 or later is flagged **(C++20+ — informational only)** and is *not* treated as core C++17 content.

### Primary sources (ground truth)
- https://en.cppreference.com/
- https://eel.is/c++draft/ (ISO C++ standard draft)
- https://isocpp.org/
- https://www.learncpp.com/

Every subsection ends with its source in the form: *(Source: https://...)*

### Conventions
- All code compiles with: `g++ -std=c++17 -Wall -Wextra -pedantic`
- ⏱️ = time estimate · ⚠️ = warning · 💡 = tip
- Each concept includes: short explanation → syntax block → minimal compileable example → common pitfalls/notes → source URL.

---

## The 5 Tiers at a Glance

- **🟢 Tier 1 — Ground Zero.** For someone who has never compiled C++. Demystify the compiler (preprocess → compile → link), 
  get a toolchain running, and cover the irreducible basics: variables, types, control flow, functions, console I/O. 
  *Goal: write and run a correct small program.*

- **🟡 Tier 2 — Foundations.** Build the mental model of memory and identity: arrays/strings, pointers vs references, 
  stack/heap, RAII, value vs reference semantics. *Goal: reason about where data lives and who owns it.*

- **🟠 Tier 3 — Object-Oriented Programming.** Designing types: lifetime (Rule of 5), inheritance, polymorphism, casts, 
   and SOLID/composition discipline. *Goal: model real domains with safe, maintainable types.*

- **🔵 Tier 4 — Modern C++17 & Advanced Language.** Production idioms: move semantics, smart pointers, 
   templates/variadics/fold expressions, lambdas, and C++17 vocabulary types. *Goal: write performant, 
   ownership-correct modern code.*

- **🔴 Tier 5 — STL Mastery & Professional Toolkit.** Library-author depth: container internals, iterators, 
  algorithms, concurrency, atomics, futures, parallel STL, type traits/`constexpr`, and tooling. 
  *Goal: pick the right tool with complexity and cost in mind.*

---

## Table of Contents

- [How to Use This Document](#how-to-use-this-document)
- [Sources & Conventions](#sources--conventions)

### 🟢 [Tier 1 — Ground Zero (Absolute Beginner)](#tier-1)
- 1.0 Prerequisites · Learning Goals · ⏱️ Study Time
- 1.1 What Is C++? The Compilation Pipeline (preprocess → compile → link)
- 1.2 Setting Up the Toolchain (GCC / Clang / MSVC + CMake basics)
- 1.3 Your First Program: Hello World
- 1.4 Variables & Primitive Types
- 1.5 `auto` and `decltype`
- 1.6 Operators & Expressions
- 1.7 Control Flow: `if`, `switch`, `for`, `while`, `do-while`
- 1.8 Functions: Declaration, Definition, Parameters, Return, Overloading
- 1.9 I/O Basics: `std::cin`, `std::cout`, `std::cerr`
- 1.10 Headers, `#include`, and the Preprocessor
- 1.11 `const`, Namespaces, and `using` Declarations

### 🟡 [Tier 2 — Foundations (Confident Beginner)](#tier-2)
- 2.0 Prerequisites · Learning Goals · ⏱️ Study Time
- 2.1 Arrays and C-Strings
- 2.2 `std::string` and `std::string_view`
- 2.3 Pointers: `*`, `&`, `nullptr`, Pointer Arithmetic, `void*`
- 2.4 References vs Pointers
- 2.5 Dynamic Memory: `new` / `delete` (and why to avoid them)
- 2.6 `struct` vs `class` Basics
- 2.7 Enums: `enum` and `enum class`
- 2.8 Function Pointers and `std::function`
- 2.9 Header Guards and `#pragma once`
- 2.10 Stack vs Heap & RAII Intro
- 2.11 Value Semantics vs Reference Semantics

### 🟠 [Tier 3 — Object-Oriented Programming (Intermediate)](#tier-3)
- 3.0 Prerequisites · Learning Goals · ⏱️ Study Time
- 3.1 Classes: Data Members, Member Functions, Access Specifiers
- 3.2 Constructors & Destructors
- 3.3 Copy / Move Constructors and the Rule of 5
- 3.4 `this` Pointer, `static` Members, `const` Member Functions
- 3.5 Operator Overloading
- 3.6 Inheritance: public / protected / private, `final`, `override`
- 3.7 Polymorphism: Virtual Functions, vtable, Dynamic Dispatch
- 3.8 Abstract Classes & Pure Virtual Functions (`= 0`)
- 3.9 Multiple Inheritance, Virtual Inheritance, the Diamond Problem
- 3.10 The Casts: `dynamic_cast`, `static_cast`, `reinterpret_cast`, `const_cast`
- 3.11 SOLID Principles Applied to C++
- 3.12 Composition vs Inheritance

### 🔵 [Tier 4 — Modern C++17 & Advanced Language (Senior)](#tier-4)
- 4.0 Prerequisites · Learning Goals · ⏱️ Study Time
- 4.1 Move Semantics, Rvalue References (`&&`), `std::move`, Perfect Forwarding
- 4.2 Smart Pointers: `unique_ptr`, `shared_ptr`, `weak_ptr`, `make_unique` / `make_shared`
- 4.3 Templates: Function Templates, Class Templates, Specialization
- 4.4 Variadic Templates & Fold Expressions (C++17)
- 4.5 SFINAE Basics & `std::enable_if`
- 4.6 Concepts Intro *(C++20+ — informational only)*
- 4.7 Lambda Expressions: Capture Modes, Generic Lambdas
- 4.8 `std::tuple`, `std::pair`, Structured Bindings (C++17)
- 4.9 `std::optional`, `std::variant`, `std::any` (C++17)
- 4.10 `std::filesystem` (C++17)
- 4.11 `if` / `switch` with Initializer (C++17)
- 4.12 Inline Variables (C++17)
- 4.13 Attributes: `[[nodiscard]]`, `[[maybe_unused]]`, `[[fallthrough]]`
- 4.14 Exceptions: `try` / `catch` / `throw`, Custom Exceptions, `noexcept`

### 🔴 [Tier 5 — STL Mastery & Professional Toolkit (Expert)](#tier-5)
- 5.0 Prerequisites · Learning Goals · ⏱️ Study Time
- 5.1 Sequence Containers: `vector`, `deque`, `list`, `forward_list`, `array`
- 5.2 Associative Containers: `set`, `map`, `multiset`, `multimap`
- 5.3 Unordered Containers: `unordered_set`, `unordered_map`, Hash Policies
- 5.4 Iterator Categories (input / output / forward / bidirectional / random-access)
- 5.5 Allocators (`std::allocator`, custom allocators)
- 5.6 Algorithms (`<algorithm>`): Sorting, Searching, Transforming, Partitioning
- 5.7 Numeric Algorithms (`<numeric>`): `accumulate`, `reduce`, `inner_product`
- 5.8 Function Objects, Binders (`bind`), `std::invoke`
- 5.9 Concurrency: `std::thread`, `std::mutex`, `lock_guard`, `unique_lock`, `condition_variable`
- 5.10 Atomics: `std::atomic`, `memory_order`
- 5.11 Futures & Promises: `std::async`, `std::future`, `std::packaged_task`
- 5.12 Parallel STL (C++17 execution policies — overview)
- 5.13 Type Traits (`<type_traits>`) & Metaprogramming Intro
- 5.14 Compile-Time Computation: `constexpr`, `consteval` *(C++20+ — informational only)*, `std::integral_constant`
- 5.15 Debugging & Sanitizers (ASan / TSan / UBSan), Static Analysis
- 5.16 Build Systems (CMake) & Package Managers (Conan / vcpkg)

### 📎 Appendices
- A. [Cheat Sheet (1 page, dense, by tier)](#appendix-cheatsheet)
- B. [12-Week Study Plan (tiers → weeks)](#appendix-study-plan)
- C. [100 Essential C++17 Interview Questions](#appendix-interview)
- D. [Further Reading (Stroustrup, Meyers, Josuttis, Sutter, Gottschling)](#appendix-reading)

<a id="tier-1"></a>
---

# 🟢 Tier 1 — Ground Zero (Absolute Beginner) -------------------------------------------------------------------

### 1.0 Prerequisites · Learning Goals · ⏱️ Study Time

**Prerequisites:** None. You should be able to use a terminal/command prompt and a text editor. No prior C++ — 
or any programming — is assumed.

**Learning goals.** After this tier you can:
- Explain what a compiler does and how source becomes an executable.
- Install a working toolchain and compile from the command line.
- Declare variables of the right type, write expressions and control flow.
- Define and call functions, including overloads.
- Read from and write to the console.
- Understand `#include`, `const`, and namespaces well enough to read real code.

**⏱️ Estimated study time:** ~12–16 hours (about 1.5 weeks at a relaxed pace).

> 💡 Type every example by hand and run it. Reading C++ is not the same as compiling it.

---

<a id="t1-pipeline"></a>
## 1.1 What Is C++? The Compilation Pipeline

C++ is a statically typed, compiled, multi-paradigm language: your human-readable source code is translated 
*ahead of time* into native machine code. Translation happens in stages — **preprocessing**, **compilation**, 
and **linking** — and understanding these stages explains most beginner errors ("undefined reference", 
"no such file", "redefinition").

```
your.cpp ──preprocess──▶ translation unit ──compile──▶ your.o (object file)
                                                            │
                                  other .o files + libs ────┼──link──▶ executable
```

- **Preprocess:** handles lines starting with `#` (e.g. `#include`, `#define`). It textually pastes headers 
in and expands macros. Output is a single "translation unit".

- **Compile:** turns one translation unit into an object file (`.o` / `.obj`) of machine code, with 
unresolved references to things defined elsewhere.

- **Link:** stitches object files and libraries together, resolves those references, and produces the final 
executable.

**Syntax / commands**

```bash
g++ -E hello.cpp -o hello.i     # stop after preprocessing
g++ -S hello.cpp -o hello.s     # stop after compile-to-assembly
g++ -c hello.cpp -o hello.o     # compile to object file (no link)
g++ hello.cpp -o hello          # do everything: preprocess, compile, link
```

**Minimal example** — observe the stages:

```bash
# Produce an object file, then link it separately
g++ -std=c++17 -Wall -Wextra -pedantic -c hello.cpp -o hello.o
g++ hello.o -o hello
./hello
```

**Common pitfalls / Notes**
- ⚠️ *"undefined reference to ..."* is a **linker** error: the compiler saw a declaration but no definition 
  was provided to the linker. It is **not** a syntax error.

- ⚠️ *"No such file or directory"* for a header is a **preprocessor** error (`#include` couldn't find the file).

- 💡 Compilation is **per translation unit**. Each `.cpp` is compiled independently; they only meet at link time.

*(Source: https://en.cppreference.com/w/cpp/language/translation_phases)*

**➤ Try it yourself:** Run the four `g++` commands above on a Hello World file and open `hello.i` in your editor. 
Find where the contents of `<iostream>` were pasted in.

---

<a id="t1-toolchain"></a>
## 1.2 Setting Up the Toolchain (GCC / Clang / MSVC + CMake basics)

A "toolchain" is the compiler plus the linker and standard library. The three mainstream options are **GCC** (`g++`), 
**Clang** (`clang++`), and **MSVC** (`cl.exe`, via Visual Studio). For multi-file projects you'll also want a build 
generator like **CMake** so you don't type compiler commands by hand.

**Install (pick one platform)**

```bash
# Windows: MSYS2 (gives you GCC), or install Visual Studio (MSVC).
#   pacman -S mingw-w64-ucrt-x86_64-gcc      # in the MSYS2 UCRT64 shell
# macOS:
xcode-select --install                       # Clang
brew install gcc cmake
# Debian/Ubuntu:
sudo apt install g++ cmake
```

**Verify the compiler**

```bash
g++ --version
g++ -std=c++17 -Wall -Wextra -pedantic hello.cpp -o hello
```

**Minimal CMake project**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        # use -std=c++17, not -std=gnu++17

add_executable(hello hello.cpp)
```

```bash
cmake -S . -B build      # configure
cmake --build build      # compile + link
./build/hello            # run (Windows: build\Debug\hello.exe)
```

**Common pitfalls / Notes**
- ⚠️ On Windows the produced binary is `hello.exe`, and CMake's default (Visual Studio) generator puts it 
  in a config subfolder like `build/Debug/`.

- 💡 `CMAKE_CXX_EXTENSIONS OFF` forces standards-conforming `-std=c++17` instead of the GNU dialect — 
  better for portable, correct code.

- 💡 Always compile with `-Wall -Wextra -pedantic` while learning. Warnings catch real bugs.

*(Source: https://cmake.org/cmake/help/latest/guide/tutorial/index.html)*

**➤ Try it yourself:** Create the `CMakeLists.txt` above next to a `hello.cpp`, then build and run it 
using only the two `cmake` commands.

---

<a id="t1-hello"></a>
## 1.3 Your First Program: Hello World

Every C++ program starts execution at a function named `main`, which returns an `int` exit code (`0` means success). 
`#include <iostream>` brings in the stream I/O facilities; `std::cout` is the standard output stream.

**Syntax**

```cpp
#include <iostream>

int main() {
    // statements
    return 0;
}
```

**Minimal compileable example**

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, World!\n";
    return 0;
}
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic hello.cpp -o hello && ./hello
# Output: Hello, World!
```

**Common pitfalls / Notes**
- 💡 You may omit `return 0;` in `main` specifically — the standard treats it as returning `0`. 
   Every other non-`void` function **must** return a value.

- ⚠️ `std::endl` flushes the stream every time; prefer `'\n'` in loops for performance and use `std::endl` 
   only when you truly need an immediate flush.

- ⚠️ `void main()` is **not** standard C++. Always use `int main()`.

*(Source: https://en.cppreference.com/w/cpp/language/main_function)*

**➤ Try it yourself:** Make the program print three separate lines, then change the return value to `1` 
  and inspect the exit code (`echo $?` on Linux/macOS, `echo %ERRORLEVEL%` on Windows cmd).

---

<a id="t1-variables"></a>
## 1.4 Variables & Primitive Types

A variable is a named, typed storage location. C++ has fundamental types for integers (`int`, `short`, `long`, `long long`, plus `unsigned` variants), characters (`char`), floating point (`float`, `double`, `long double`), and `bool`. Prefer **brace initialization** `{}` — it forbids narrowing conversions that silently lose data.

**Syntax**

```cpp
type name{value};   // preferred (list-initialization)
type name = value;  // copy-initialization
type name(value);   // direct-initialization
```

**Minimal compileable example**

```cpp
#include <cstdint>
#include <iostream>

int main() {
    int    count{42};
    double price{9.99};
    char   grade{'A'};
    bool   ready{true};
    std::int64_t big{9'000'000'000};   // fixed-width type; ' is a digit separator

    std::cout << count << ' ' << price << ' ' << grade << ' '
              << ready << ' ' << big << '\n';
    // int x{3.14};  // ⚠️ ERROR: narrowing double->int is rejected by {}
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Sizes of `int`, `long`, etc. are **implementation-defined** (only minimums are guaranteed). When you need an exact width, use `<cstdint>` types like `std::int32_t`, `std::uint64_t`.
- ⚠️ Reading an **uninitialized** local variable is undefined behavior. Brace-initialize: `int n{};` zero-initializes.
- ⚠️ Mixing signed and unsigned in comparisons/arithmetic causes surprising results (`-1 < 1u` is *false*). `-Wextra` warns about this.

*(Source: https://en.cppreference.com/w/cpp/language/types)*

**➤ Try it yourself:** Declare an `unsigned int u{0}` and print `u - 1`. Explain the giant number you get.

---

<a id="t1-auto"></a>
## 1.5 `auto` and `decltype`

`auto` asks the compiler to deduce a variable's type from its initializer; `decltype(expr)` yields the *declared type* of an expression without evaluating it. Both reduce noise and prevent accidental type mismatches, but `auto` drops top-level `const` and references unless you add them back.

**Syntax**

```cpp
auto x = expr;            // type deduced from expr
const auto& r = expr;     // deduce, but keep const + bind by reference
decltype(expr) y = expr;  // y has the exact declared type of expr
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>

int main() {
    auto i = 5;             // int
    auto d = 5.0;           // double
    auto s = std::string{"hi"};  // std::string, not const char*

    int n = 10;
    const int& cr = n;
    auto a = cr;            // a is int (const & dropped) -> a copy
    decltype(cr) b = n;     // b is const int& (exact type kept)

    a = 99;                 // OK, a is an independent int
    std::cout << i << ' ' << d << ' ' << s << ' ' << b << '\n';
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `auto x = expr;` copies and strips `const`/`&`. Use `auto&` / `const auto&` when you want to refer to the original object.
- ⚠️ `auto` cannot deduce from a brace list the way you'd expect: `auto z{1};` is `int`, but `auto z = {1};` is `std::initializer_list<int>`.
- 💡 Use `auto` to avoid spelling out long iterator/template types; don't overuse it where it hides an important type from the reader.

*(Source: https://en.cppreference.com/w/cpp/language/auto)*

**➤ Try it yourself:** Use `decltype` to declare a variable with the same type as `3 + 4.5`. What type is it, and why?

---

<a id="t1-operators"></a>
## 1.6 Operators & Expressions

Operators combine values into expressions: arithmetic (`+ - * / %`), comparison (`== != < <= > >=`), logical (`&& || !`), assignment (`= += ...`), increment/decrement (`++ --`), and bitwise (`& | ^ ~ << >>`). Each operator has a **precedence** and **associativity** that determine grouping; use parentheses when in doubt.

**Syntax**

```cpp
a + b * c          // * binds tighter than +
a && b || c        // && binds tighter than ||
x = y = 0          // assignment is right-associative
```

**Minimal compileable example**

```cpp
#include <iostream>

int main() {
    int a{7}, b{2};
    std::cout << a / b << '\n';   // 3  (integer division truncates)
    std::cout << a % b << '\n';   // 1  (remainder)
    std::cout << (a > b && b > 0) << '\n';  // 1 (true)

    double x{7.0};
    std::cout << x / b << '\n';   // 3.5 (one operand is double)

    int i{5};
    std::cout << i++ << ' ' << i << '\n';  // 5 6 (post-increment)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Integer division **truncates toward zero**: `7 / 2 == 3`. Make an operand floating point for real division.
- ⚠️ `=` (assignment) vs `==` (comparison) is the classic bug. `-Wall` warns when you write `if (x = 5)`.
- ⚠️ Modifying a variable more than once between sequence points (e.g. `i = i++ + ++i;`) is undefined behavior. Don't.

*(Source: https://en.cppreference.com/w/cpp/language/operator_precedence)*

**➤ Try it yourself:** Predict then verify the output of `2 + 3 * 4`, `(2 + 3) * 4`, and `10 % 3`.

---

<a id="t1-control"></a>
## 1.7 Control Flow: `if`, `switch`, `for`, `while`, `do-while`

Control-flow statements decide which code runs and how often. `if`/`else` branches on a condition; `switch` selects among integer/enum cases; `for`, `while`, and `do-while` loop. Use `for` when the iteration count is known, `while` when looping until a condition changes, and `do-while` when the body must run at least once.

**Syntax**

```cpp
if (cond) { ... } else if (cond2) { ... } else { ... }

switch (n) { case 1: ...; break; default: ...; }

for (init; cond; step) { ... }
while (cond) { ... }
do { ... } while (cond);
```

**Minimal compileable example**

```cpp
#include <iostream>

int main() {
    for (int i = 0; i < 3; ++i)
        std::cout << "for " << i << '\n';

    int n = 3;
    while (n > 0) { std::cout << "while " << n << '\n'; --n; }

    int day = 2;
    switch (day) {
        case 1: std::cout << "Mon\n"; break;
        case 2: std::cout << "Tue\n"; break;
        default: std::cout << "other\n"; break;
    }
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Forgetting `break` in a `switch` causes **fall-through** to the next case. If intentional, mark it with `[[fallthrough]];` (Tier 4) to silence warnings.
- ⚠️ A stray semicolon — `for (...);` or `while (cond);` — makes an empty loop body. `-Wextra` may catch some cases.
- 💡 Prefer the **range-based for** for containers: `for (const auto& x : v) { ... }` (covered with containers in Tier 5).

*(Source: https://en.cppreference.com/w/cpp/language/statements)*

**➤ Try it yourself:** Write a `for` loop printing 1–10, but skip multiples of 3 using `continue`.

---

<a id="t1-functions"></a>
## 1.8 Functions: Declaration, Definition, Parameters, Return, Overloading

A function packages reusable logic. A **declaration** (prototype) tells the compiler the name, parameters, and return type; a **definition** provides the body. Parameters are passed **by value** (a copy) by default; pass by `const&` to avoid copying large objects. **Overloading** lets multiple functions share a name if their parameter lists differ.

**Syntax**

```cpp
ret_type name(param_type param, ...);          // declaration
ret_type name(param_type param, ...) { ... }   // definition
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>

int  square(int x) { return x * x; }                  // by value
void greet(const std::string& who) {                  // by const reference
    std::cout << "Hi, " << who << '\n';
}

// Overloads: same name, different parameter types
double area(double side)        { return side * side; }       // square
double area(double w, double h) { return w * h; }             // rectangle

int main() {
    std::cout << square(6) << '\n';     // 36
    greet("Ada");
    std::cout << area(3.0) << ' ' << area(3.0, 4.0) << '\n';  // 9 12
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A function used before it's defined needs a **declaration** earlier (typically in a header). Otherwise the compiler doesn't know it.
- ⚠️ Overloads must differ in **parameters**, not just return type: two functions differing only by return type is a compile error.
- 💡 Pass big objects (`std::string`, containers) by `const&` to avoid expensive copies; pass small types (`int`, `double`) by value.
- 💡 Default arguments go in the **declaration**, not the definition: `int f(int x = 0);`.

*(Source: https://en.cppreference.com/w/cpp/language/functions)*

**➤ Try it yourself:** Add a third `area` overload taking three doubles (a triangle: ½·base·height) and call all three.

---

<a id="t1-io"></a>
## 1.9 I/O Basics: `std::cin`, `std::cout`, `std::cerr`

`<iostream>` provides three standard streams: `std::cout` (buffered standard output), `std::cerr` (unbuffered standard error, for diagnostics), and `std::cin` (standard input). Use `<<` to insert into output streams and `>>` to extract from input streams.

**Syntax**

```cpp
std::cout << value << value;   // insertion
std::cin  >> variable;          // extraction (whitespace-delimited)
std::getline(std::cin, str);    // read a whole line into a std::string
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>

int main() {
    std::cout << "Enter your age: ";
    int age{};
    if (std::cin >> age) {
        std::cout << "Next year you'll be " << age + 1 << '\n';
    } else {
        std::cerr << "That wasn't a number.\n";   // goes to stderr
        return 1;
    }
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `std::cin >> x` stops at whitespace, so it can't read "two words". Use `std::getline` for full lines.
- ⚠️ Mixing `>>` and `getline` leaves a leftover newline in the buffer; clear it with `std::cin.ignore()` before `getline`.
- ⚠️ Always check the stream state (`if (std::cin >> x)`) — failed extraction leaves the variable unchanged and sets a fail flag.
- 💡 Diagnostics belong on `std::cerr` so they aren't swallowed when stdout is redirected to a file.

*(Source: https://en.cppreference.com/w/cpp/io/cin)*

**➤ Try it yourself:** Read the user's full name with `std::getline` and greet them. Then read two integers and print their sum.

---

<a id="t1-headers"></a>
## 1.10 Headers, `#include`, and the Preprocessor

The preprocessor runs before compilation and obeys directives starting with `#`. `#include` pastes a header's contents in place; `#define` creates macros; `#ifdef`/`#endif` conditionally include code. Standard library headers use angle brackets `<...>`; your own project headers use quotes `"..."`.

**Syntax**

```cpp
#include <iostream>     // standard library / system header
#include "myutils.h"    // your own header (searched locally first)
#define MAX_USERS 100   // object-like macro (avoid; prefer constexpr)
#ifdef DEBUG
    // compiled only when DEBUG is defined
#endif
```

**Minimal compileable example** (two files)

```cpp
// mathutils.h
#pragma once
int add(int a, int b);     // declaration only
```

```cpp
// main.cpp
#include <iostream>
#include "mathutils.h"

int add(int a, int b) { return a + b; }   // definition

int main() {
    std::cout << add(2, 3) << '\n';        // 5
    return 0;
}
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o app && ./app
```

**Common pitfalls / Notes**
- ⚠️ Macros are blunt text substitution with no type checking. Prefer `constexpr` constants and `inline` functions to `#define` in modern C++.
- ⚠️ Including a header **twice** can cause "redefinition" errors — guard headers (next section).
- 💡 Headers should contain **declarations** (and inline/template definitions), while `.cpp` files contain the **definitions**. This keeps the linker happy.

*(Source: https://en.cppreference.com/w/cpp/preprocessor)*

**➤ Try it yourself:** Add a `subtract` declaration to `mathutils.h`, define it in `main.cpp`, and call it.

---

<a id="t1-const-ns"></a>
## 1.11 `const`, Namespaces, and `using` Declarations

`const` marks something as immutable — the compiler rejects attempts to modify it, documenting intent and enabling optimizations. **Namespaces** group names to prevent collisions (the standard library lives in `std`). A `using` *declaration* (`using std::cout;`) brings one name into scope; the `using` *directive* (`using namespace std;`) brings them all — avoid the latter in headers and at large scope.

**Syntax**

```cpp
const int kMax = 100;          // cannot be reassigned
namespace app { int value; }   // define a namespace
app::value = 5;                // qualified access
using app::value;              // bring one name into scope
```

**Minimal compileable example**

```cpp
#include <iostream>

namespace geometry {
    const double kPi = 3.14159265358979;
    double circumference(double r) { return 2 * kPi * r; }
}

int main() {
    using geometry::circumference;        // declaration: just this name
    std::cout << circumference(1.0) << '\n';   // 6.28319
    std::cout << geometry::kPi << '\n';        // fully qualified
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `const` objects **must** be initialized at declaration — there's no later chance to set them.
- ⚠️ Avoid `using namespace std;` in headers and at global scope: it pulls thousands of names into every file that includes it, inviting ambiguities. A localized `using std::cout;` inside a function is fine.
- 💡 Read `const` right-to-left: `const int* p` is "pointer to const int"; `int* const p` is "const pointer to int" (more in Tier 2).
- 💡 Prefer `constexpr` over `const` for compile-time constants when the value is known at compile time (Tier 5).

*(Source: https://en.cppreference.com/w/cpp/language/namespace)*

**➤ Try it yourself:** Put two functions named `area` in two different namespaces and call each with its qualified name.

---

### ✅ Tier 1 Checkpoint

You can now: compile from source, choose appropriate primitive types, write expressions/control flow, define and overload functions, do console I/O, and read code using headers, `const`, and namespaces.

**Self-test:** Write a program that reads an integer `n`, then prints whether it's even or odd and the sum `1 + 2 + ... + n` using a loop and a helper function.

---

<a id="tier-2"></a>
---

=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================


# 🟡 Tier 2 — Foundations (Confident Beginner)

### 2.0 Prerequisites · Learning Goals · ⏱️ Study Time

**Prerequisites (from Tier 1):** variables and primitive types, functions and overloading, control flow, `const`, headers/`#include`, and basic console I/O. You should be comfortable compiling with `g++ -std=c++17 -Wall -Wextra -pedantic`.

**Learning goals.** After this tier you can:
- Use arrays, C-strings, `std::string`, and `std::string_view` correctly.
- Reason about pointers, references, and the differences between them.
- Explain where objects live (stack vs heap) and who owns them.
- Use `new`/`delete` — and explain why you should rarely do so.
- Choose between `struct` and `class`, and between scoped and unscoped enums.
- Store callable things with function pointers and `std::function`.
- Apply RAII thinking and distinguish value from reference semantics.

**⏱️ Estimated study time:** ~16–20 hours (about 2 weeks).

> 💡 This is the tier where the *memory model* clicks. Don't rush it — almost every C++ bug later traces back to a fuzzy understanding of ownership and lifetime.

---

<a id="t2-arrays"></a>
## 2.1 Arrays and C-Strings

A built-in array is a fixed-size, contiguous block of elements of one type, named at compile time. A **C-string** is just a `char` array terminated by a null byte `'\0'`. Built-in arrays don't know their own size (they "decay" to a pointer when passed to functions), which is why modern C++ prefers `std::array` (Tier 5) and `std::string`.

**Syntax**

```cpp
T arr[N];                 // N elements, uninitialized (if local)
T arr[]{a, b, c};         // size deduced from initializer (=3)
const char* s = "hello";  // pointer to a read-only C-string literal
char buf[]{"hi"};         // char array: {'h','i','\0'} -> size 3
```

**Minimal compileable example**

```cpp
#include <cstring>   // std::strlen
#include <iostream>

int main() {
    int nums[]{10, 20, 30};                 // size 3 deduced
    std::size_t n = std::size(nums);        // C++17: array size, type-safe
    for (std::size_t i = 0; i < n; ++i)
        std::cout << nums[i] << ' ';
    std::cout << '\n';

    const char* greeting = "hello";         // null-terminated
    std::cout << greeting << " has length "
              << std::strlen(greeting) << '\n';   // 5 (excludes '\0')
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **No bounds checking.** `nums[3]` above is undefined behavior — it reads past the array. This is the classic buffer overrun.
- ⚠️ Arrays **decay to pointers** when passed to functions, losing size info. Either pass the size too, or use `std::array`/`std::vector`.
- ⚠️ `sizeof(arr)` gives total bytes only where the array type is visible; after decay, `sizeof(ptr)` is just the pointer size. C++17's `std::size(arr)` is the safe way to get element count.
- 💡 Prefer `std::string` over `char[]` and `std::array`/`std::vector` over `T[]` in almost all new code.

*(Source: https://en.cppreference.com/w/cpp/language/array)*

**➤ Try it yourself:** Make a `double` array of 5 temperatures and compute their average using `std::size`.

---

<a id="t2-string"></a>
## 2.2 `std::string` and `std::string_view`

`std::string` is an owning, growable, null-handling string class — it manages its own heap buffer so you never deal with `'\0'` or manual sizing. `std::string_view` (C++17) is a non-owning, read-only *view* over an existing character sequence (a `std::string`, a literal, or a buffer): cheap to copy, but it must not outlive the data it points at.

**Syntax**

```cpp
#include <string>
#include <string_view>

std::string s{"hello"};          // owns its characters
s += " world";                    // grows automatically
std::string_view sv{s};           // views s's data, copies nothing
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>
#include <string_view>

// Takes a view: works for std::string, literals, char arrays — no copy.
void print_len(std::string_view sv) {
    std::cout << sv << " -> " << sv.size() << '\n';
}

int main() {
    std::string name{"Ada"};
    name += " Lovelace";          // owning string grows
    print_len(name);              // "Ada Lovelace" -> 12
    print_len("literal");         // no std::string temporary created
    std::cout << name.substr(0, 3) << '\n';   // "Ada"
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **Dangling views.** Never return a `string_view` to a local string, and never store a view past the lifetime of its source: `std::string_view bad = std::string{"tmp"};` dangles immediately.
- ⚠️ `string_view` is **not** guaranteed null-terminated. Don't pass `sv.data()` to C APIs expecting a `char*` C-string.
- 💡 Use `string_view` for **read-only function parameters** — it accepts strings and literals without copying. Use `std::string` when you need to **own or modify** text.
- 💡 Prefer `+=` and `reserve()` over building strings with repeated `+` in loops to avoid repeated reallocation.

*(Source: https://en.cppreference.com/w/cpp/string/basic_string_view)*

**➤ Try it yourself:** Write a function `bool starts_with(std::string_view s, std::string_view prefix)` and test it with a literal and a `std::string`.

---

<a id="t2-pointers"></a>
## 2.3 Pointers: `*`, `&`, `nullptr`, Pointer Arithmetic, `void*`

A pointer holds the **address** of an object. `&x` takes the address of `x`; `*p` dereferences `p` to access the pointed-to object. `nullptr` is the typed null pointer constant (use it, never `NULL` or `0`). Adding to a pointer moves it by *whole elements*, not bytes. A `void*` is a typeless address that must be cast back before use.

**Syntax**

```cpp
T* p = &x;       // p points to x
*p = 5;          // write through the pointer
T* q = nullptr;  // points to nothing
p + 1;           // address of the next T (pointer arithmetic)
```

**Minimal compileable example**

```cpp
#include <iostream>

int main() {
    int x{42};
    int* p = &x;           // p holds address of x
    std::cout << *p << '\n';     // 42 (read through pointer)
    *p = 100;                    // write through pointer
    std::cout << x << '\n';      // 100 (x changed)

    int arr[]{10, 20, 30};
    int* it = arr;               // points to arr[0]
    std::cout << *(it + 2) << '\n';  // 30 (pointer arithmetic)

    int* none = nullptr;
    if (none == nullptr) std::cout << "points to nothing\n";
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **Dereferencing `nullptr` or a dangling pointer is undefined behavior** — usually a crash, sometimes silent corruption. Always check or guarantee validity.
- ⚠️ Pointer arithmetic is only valid **within a single array** (and one-past-the-end). Going further is UB.
- ⚠️ `void*` loses type information; you must `static_cast` it back to the correct type before dereferencing. Reinterpreting it as the wrong type is UB.
- 💡 Use `nullptr`, not `NULL`/`0` — it has a distinct type (`std::nullptr_t`) and overloads/templates resolve correctly.

*(Source: https://en.cppreference.com/w/cpp/language/pointer)*

**➤ Try it yourself:** Write `void increment(int* p)` that adds 1 to `*p`, and call it so a local variable changes.

---

<a id="t2-refs"></a>
## 2.4 References vs Pointers

A reference is an **alias** for an existing object: once bound, it always refers to that same object and can never be null or rebound. Pointers are more flexible (nullable, reassignable, support arithmetic) but require explicit dereferencing and validity checks. Prefer references for parameters that must always refer to a real object; use pointers when "no object" (`nullptr`) is a valid state or when you need to reseat.

**Syntax**

```cpp
T& r = x;          // r is another name for x (must be initialized)
const T& cr = x;   // read-only alias (also binds to temporaries)
void f(T& out);    // caller's object can be modified
void g(const T& in); // read-only, no copy
```

**Minimal compileable example**

```cpp
#include <iostream>

void add_ref(int& a, int b) { a += b; }     // modifies caller's variable
void add_ptr(int* a, int b) { if (a) *a += b; }  // must check null

int main() {
    int x{10};
    int& alias = x;     // alias and x are the same object
    alias = 20;
    std::cout << x << '\n';   // 20

    add_ref(x, 5);            // clean syntax, no &/* at call site... wait:
    std::cout << x << '\n';   // 25
    add_ptr(&x, 5);           // explicit address-of; can pass nullptr
    std::cout << x << '\n';   // 30
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A reference **must be initialized** when declared and **cannot be rebound** afterward. `T& r;` is an error.
- ⚠️ Don't return a reference to a **local** variable — it dangles after the function returns.
- 💡 `const T&` parameters avoid copies *and* can bind to temporaries (`f(std::string{"x"})`), making them ideal for read-only input.
- 💡 Rule of thumb: **reference** when it must always be valid; **pointer** when absence (`nullptr`) is meaningful or you must point at different objects over time.

*(Source: https://en.cppreference.com/w/cpp/language/reference)*

**➤ Try it yourself:** Write `void swap_ints(int& a, int& b)` using references and verify two variables exchange values.

---

<a id="t2-newdelete"></a>
## 2.5 Dynamic Memory: `new` / `delete` (and why to avoid them)

`new` allocates an object on the **heap** and returns a pointer; `delete` frees it. The heap lets objects outlive the scope that created them, but *you* become responsible for freeing exactly once — forget and you leak; free twice or use after free and you corrupt memory. Modern C++ replaces raw `new`/`delete` with **smart pointers** (Tier 4) and containers, which free automatically.

**Syntax**

```cpp
T* p = new T(args);     // single object
delete p;               // free single object

T* a = new T[n];        // array
delete[] a;             // free array  (must match new[])
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <memory>     // std::make_unique (the modern way)

int main() {
    // Raw form (educational — note the manual delete):
    int* p = new int{42};
    std::cout << *p << '\n';
    delete p;                       // required, exactly once
    p = nullptr;                    // avoid dangling reuse

    // Modern form: frees itself, no leak possible:
    auto up = std::make_unique<int>(42);
    std::cout << *up << '\n';
    return 0;                       // up's memory released automatically
}
```

**Common pitfalls / Notes**
- ⚠️ **Mismatched forms** are UB: use `delete` for `new`, `delete[]` for `new[]`. Never mix.
- ⚠️ **Leaks** (never `delete`), **double-free** (`delete` twice), and **use-after-free** (deref after `delete`) are the classic heap bugs. An early `return`/exception between `new` and `delete` leaks.
- ⚠️ A raw owning pointer gives no hint about *who* frees it. Ownership becomes guesswork.
- 💡 **Prefer `std::make_unique` / `std::make_shared` and containers.** In modern C++ you should almost never write a bare `new`/`delete`. (Smart pointers: Tier 4.)

*(Source: https://en.cppreference.com/w/cpp/language/new)*

**➤ Try it yourself:** Allocate an `int` with `new`, print it, `delete` it — then rewrite the program with `std::make_unique` and note what you removed.

---

<a id="t2-struct-class"></a>
## 2.6 `struct` vs `class` Basics

Both `struct` and `class` define a user-defined type bundling data and functions. The **only** language difference is the default access level: `struct` members are **public** by default, `class` members are **private**. By convention, use `struct` for simple passive data aggregates and `class` for types with invariants/behavior to encapsulate.

**Syntax**

```cpp
struct Point { int x; int y; };          // members public by default

class Counter {
    int count_{0};                       // private by default
public:
    void tick() { ++count_; }
    int value() const { return count_; }
};
```

**Minimal compileable example**

```cpp
#include <iostream>

struct Point { int x; int y; };          // aggregate: just data

class Counter {
    int count_{0};                       // hidden state (private)
public:
    void tick() { ++count_; }
    int value() const { return count_; }
};

int main() {
    Point p{3, 4};                       // aggregate initialization
    std::cout << p.x << ',' << p.y << '\n';   // 3,4

    Counter c;
    c.tick(); c.tick();
    std::cout << c.value() << '\n';      // 2
    // c.count_ = 99;  // ⚠️ ERROR: count_ is private
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ The default-access difference also applies to **inheritance**: `struct D : Base` is public inheritance; `class D : Base` is private by default (Tier 3).
- 💡 Use `struct` for "plain old data"/aggregates you can brace-initialize; use `class` when you must protect invariants behind a public interface.
- 💡 Trailing-underscore (`count_`) is a common convention to mark data members; pick a style and be consistent.

*(Source: https://en.cppreference.com/w/cpp/language/classes)*

**➤ Try it yourself:** Turn `Point` into a `class` with private `x_`, `y_` and public getters; observe what breaks and fix the call site.

---

<a id="t2-enums"></a>
## 2.7 Enums: `enum` and `enum class`

An enumeration is a type with a fixed set of named integer constants. The old unscoped `enum` leaks its names into the enclosing scope and implicitly converts to `int` — a source of bugs. The C++11 **scoped** `enum class` keeps names namespaced (`Color::Red`), is strongly typed (no accidental int conversion), and lets you fix the underlying type. Prefer `enum class`.

**Syntax**

```cpp
enum Color { Red, Green, Blue };               // unscoped (leaks names)
enum class Suit : char { Hearts, Spades };     // scoped + fixed base type
Suit s = Suit::Hearts;                          // must qualify
```

**Minimal compileable example**

```cpp
#include <iostream>

enum class Direction { North, East, South, West };

const char* to_string(Direction d) {
    switch (d) {
        case Direction::North: return "North";
        case Direction::East:  return "East";
        case Direction::South: return "South";
        case Direction::West:  return "West";
    }
    return "?";
}

int main() {
    Direction d = Direction::East;
    std::cout << to_string(d) << '\n';                 // East
    // int n = d;            // ⚠️ ERROR: no implicit conversion
    int n = static_cast<int>(d);                       // explicit is fine
    std::cout << n << '\n';                             // 1
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Unscoped `enum` values implicitly convert to `int`, so two unrelated enums can be compared/mixed silently. `enum class` forbids this.
- ⚠️ A `switch` over an enum that omits a case (without `default`) triggers `-Wswitch` warnings — useful for catching missed cases when you add enumerators.
- 💡 Specify the underlying type (`: char`, `: std::uint8_t`) when size matters (e.g. serialization, packed structs).
- 💡 To get the integer value of a scoped enum, use `static_cast<int>(value)` — it's explicit on purpose.

*(Source: https://en.cppreference.com/w/cpp/language/enum)*

**➤ Try it yourself:** Add a `Direction turn_right(Direction)` function that cycles North→East→South→West→North.

---

<a id="t2-funcptr"></a>
## 2.8 Function Pointers and `std::function`

A function pointer stores the address of a function so you can call it indirectly or pass behavior as a parameter. `std::function` (from `<functional>`) is a type-erasing wrapper that can hold *any* callable with a matching signature — plain functions, lambdas with captures, or function objects — at the cost of a small overhead. Use a raw function pointer for the simplest cases; `std::function` when you need to store arbitrary callables.

**Syntax**

```cpp
int (*fp)(int, int) = &add;          // raw function pointer
std::function<int(int,int)> f = add; // wraps any matching callable
```

**Minimal compileable example**

```cpp
#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int apply(int x, int y, int (*op)(int, int)) {   // takes a function pointer
    return op(x, y);
}

int main() {
    std::cout << apply(3, 4, add) << '\n';   // 7
    std::cout << apply(3, 4, mul) << '\n';   // 12

    int factor = 10;
    std::function<int(int)> scale = [factor](int v) { return v * factor; };
    std::cout << scale(5) << '\n';           // 50 (lambda with capture)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A **capturing** lambda is *not* convertible to a plain function pointer — only captureless lambdas are. Store capturing lambdas in `std::function` (or use templates, Tier 4).
- ⚠️ `std::function` may heap-allocate and adds indirection; in hot paths prefer templates or function pointers.
- ⚠️ A `std::function` holding a lambda that captured by reference can dangle if the referenced objects die first.
- 💡 For function *parameters* that take callables, a template parameter (`template<class F> void each(F f)`) is usually faster than `std::function`.

*(Source: https://en.cppreference.com/w/cpp/utility/functional/function)*

**➤ Try it yourself:** Store three operations (`add`, `mul`, and a subtract lambda) in a `std::function<int(int,int)>` array and call each.

---

<a id="t2-guards"></a>
## 2.9 Header Guards and `#pragma once`

Because `#include` is literal text insertion, including the same header twice in one translation unit would redefine its contents — an error. A **header guard** (an `#ifndef`/`#define`/`#endif` triple) ensures the body is processed only once. `#pragma once` does the same thing in one line; it's non-standard but supported by all major compilers.

**Syntax**

```cpp
// Classic include guard
#ifndef MYPROJECT_WIDGET_H
#define MYPROJECT_WIDGET_H
// ... declarations ...
#endif

// Modern one-liner (top of file)
#pragma once
// ... declarations ...
```

**Minimal compileable example**

```cpp
// widget.h
#pragma once
struct Widget { int id; };
```

```cpp
// main.cpp
#include "widget.h"
#include "widget.h"     // included twice on purpose — guard prevents redefinition
#include <iostream>

int main() {
    Widget w{7};
    std::cout << w.id << '\n';   // 7
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ With classic guards, the macro name **must be unique** project-wide. Two headers sharing a guard macro silently hide one another — use a path-based name like `MYPROJECT_SUBDIR_FILE_H`.
- ⚠️ `#pragma once` can misbehave when the *same* file is reachable via two different paths (symlinks, copies); classic guards key on the macro name instead.
- 💡 Pick one convention per project. `#pragma once` is concise and the common modern default; classic guards are maximally portable.

*(Source: https://en.cppreference.com/w/cpp/preprocessor/impl#.23pragma_once)*

**➤ Try it yourself:** Create a header with a classic guard, include it twice from a `.cpp`, and confirm it still compiles. Then remove the guard and read the error.

---

<a id="t2-raii"></a>
## 2.10 Stack vs Heap & RAII Intro

Local objects live on the **stack**: they're created at their declaration and destroyed automatically when their scope ends, in reverse order — fast and automatic. Heap objects (from `new`) live until explicitly freed. **RAII** (Resource Acquisition Is Initialization) is the core C++ idiom: wrap a resource (memory, file, lock) in an object so its **destructor** releases it, tying cleanup to scope exit — even when exceptions are thrown.

**Syntax**

```cpp
class Guard {
public:
    Guard()  { /* acquire resource */ }
    ~Guard() { /* release resource (runs automatically) */ }
};
{
    Guard g;     // acquired here
}                // released here, guaranteed
```

**Minimal compileable example**

```cpp
#include <iostream>

class FileHandle {                 // toy RAII wrapper
    const char* name_;
public:
    explicit FileHandle(const char* name) : name_{name} {
        std::cout << "open " << name_ << '\n';     // acquire
    }
    ~FileHandle() {
        std::cout << "close " << name_ << '\n';    // release, automatic
    }
};

int main() {
    std::cout << "start\n";
    {
        FileHandle f{"data.txt"};   // opened
        std::cout << "using file\n";
    }                               // f destroyed here -> "close data.txt"
    std::cout << "end\n";
    return 0;
}
// Output: start / open data.txt / using file / close data.txt / end
```

**Common pitfalls / Notes**
- ⚠️ Stack space is limited; huge arrays or deep recursion cause **stack overflow**. Large/long-lived data belongs on the heap (via containers/smart pointers).
- ⚠️ Returning a pointer/reference to a stack object that has gone out of scope is a dangling-reference bug.
- 💡 RAII is *the* reason C++ doesn't need a `finally` block: destructors run on every exit path, including exceptions. Build cleanup into types, not into manual `delete`/`close` calls.
- 💡 The standard library is RAII all the way down: `std::string`, `std::vector`, `std::unique_ptr`, `std::lock_guard` all clean up in their destructors.

*(Source: https://en.cppreference.com/w/cpp/language/raii)*

**➤ Try it yourself:** Add a second `FileHandle` in the same scope and observe the **reverse** destruction order at scope exit.

---

<a id="t2-semantics"></a>
## 2.11 Value Semantics vs Reference Semantics

With **value semantics**, assigning or passing an object makes an independent **copy** — changing one doesn't affect the other (think `int`, `std::string`, `std::vector`). With **reference semantics**, multiple names refer to the *same* underlying object, so a change is visible through all of them (think references, pointers, `shared_ptr`). C++ defaults to value semantics, which makes code easier to reason about; reach for reference semantics deliberately.

**Syntax**

```cpp
T a = b;     // value semantics: a is a copy of b
T& r = b;    // reference semantics: r is another name for b
T* p = &b;   // reference semantics via pointer
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b = a;     // VALUE: deep copy, independent
    b[0] = 99;
    std::cout << a[0] << ' ' << b[0] << '\n';   // 1 99  (a unaffected)

    std::vector<int>& r = a;    // REFERENCE: alias for a
    r[0] = 42;
    std::cout << a[0] << '\n';  // 42  (a changed through r)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Copying large value-semantic objects (big vectors/strings) can be expensive. Pass by `const&` to read without copying; use **move** to transfer cheaply (Tier 4).
- ⚠️ Reference semantics introduces **aliasing**: a function may change data you didn't expect it to. Document and constrain who can mutate shared state.
- 💡 Default to value semantics for clarity and safety; introduce references/pointers/`shared_ptr` only when sharing or polymorphism requires it.
- 💡 This distinction is the foundation for **copy vs move constructors** and the **Rule of 5**, both covered in Tier 3.

*(Source: https://en.cppreference.com/w/cpp/language/value_category)*

**➤ Try it yourself:** Write `void zero_first(std::vector<int> v)` (by value) and `void zero_first(std::vector<int>& v)` (by reference) — call each and explain which one actually changes the caller's vector.

---

### ✅ Tier 2 Checkpoint

You can now: use arrays/strings safely, reason about pointers vs references, explain stack vs heap and ownership, use (and justify avoiding) `new`/`delete`, choose `struct`/`class` and `enum class`, store callables, guard headers, and distinguish value from reference semantics with RAII as your cleanup discipline.

**Self-test:** Build a tiny RAII `IntBuffer` class that allocates `int[n]` with `new[]` in its constructor and frees it with `delete[]` in its destructor, exposes `operator[]` and `size()`, and prove (with prints) that it cleans up at scope exit. Then note which Tier 4 tool would replace it entirely.

---

## Next Step

**Tier 2 is complete.** I'll pause here. Tell me if you want any adjustments — otherwise say **go** and 
I'll write **🟠 Tier 3 — Object-Oriented Programming** into this same file.

=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================

<a id="tier-3"></a>
---

# 🟠 Tier 3 — Object-Oriented Programming (Intermediate)

### 3.0 Prerequisites · Learning Goals · ⏱️ Study Time

**Prerequisites (from Tiers 1–2):** functions and overloading, `const`, references vs pointers, stack vs heap, RAII, value vs reference semantics, `struct` vs `class` basics, and `enum class`. You should already be comfortable writing and compiling multi-file programs.

**Learning goals.** After this tier you can:
- Design classes with proper encapsulation, constructors, and destructors.
- Implement the **Rule of 5** (copy/move) correctly and know when the Rule of 0 applies.
- Use `this`, `static` members, and `const` member functions appropriately.
- Overload operators to give types natural syntax.
- Model "is-a" relationships with inheritance and polymorphism (virtual functions, vtables).
- Build abstract interfaces with pure virtual functions.
- Navigate multiple/virtual inheritance and the diamond problem.
- Choose the correct C++ cast, and apply SOLID and composition-over-inheritance.

**⏱️ Estimated study time:** ~22–28 hours (about 2.5–3 weeks).

> 💡 OOP in C++ is about **lifetime and ownership** as much as polymorphism. If a type owns a resource, the Rule of 5 (or 0) is not optional.

---

<a id="t3-classes"></a>
## 3.1 Classes: Data Members, Member Functions, Access Specifiers

A class bundles **data members** (state) with **member functions** (behavior) and controls access through specifiers: `public` (anyone), `protected` (this class and derived classes), and `private` (this class only). Encapsulation means exposing a small, safe public interface while keeping the representation private, so you can change internals without breaking callers.

**Syntax**

```cpp
class Name {
public:                 // interface
    Name(/*args*/);
    int  query() const;
private:                // implementation detail
    int data_;
};
```

**Minimal compileable example**

```cpp
#include <iostream>

class BankAccount {
public:
    explicit BankAccount(int cents) : balance_{cents} {}

    void deposit(int cents) { balance_ += cents; }
    bool withdraw(int cents) {                 // protects the invariant
        if (cents > balance_) return false;
        balance_ -= cents;
        return true;
    }
    int balance() const { return balance_; }
private:
    int balance_{0};                            // can never go negative
};

int main() {
    BankAccount acc{100};
    acc.deposit(50);
    std::cout << acc.withdraw(200) << '\n';     // 0 (refused)
    std::cout << acc.balance() << '\n';         // 150
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Exposing data members publicly breaks encapsulation — callers depend on your representation, and you can no longer enforce invariants. Keep state `private`.
- ⚠️ Mark single-argument constructors `explicit` to prevent surprising implicit conversions (`BankAccount a = 100;` would otherwise compile).
- 💡 Default member initializers (`int balance_{0};`) ensure members are always initialized, even if a constructor forgets.
- 💡 `protected` is for inheritance; prefer `private` + protected/public member functions over exposing `protected` data.

*(Source: https://en.cppreference.com/w/cpp/language/classes)*

**➤ Try it yourself:** Add a `transfer(BankAccount& to, int cents)` member that withdraws from `*this` and deposits into `to`, failing atomically if funds are insufficient.

---

<a id="t3-ctor-dtor"></a>
## 3.2 Constructors & Destructors

A **constructor** initializes an object when it's created; a **destructor** (`~Name()`) runs automatically when it's destroyed, releasing any resources the object owns. Prefer the **member initializer list** (`: a_{x}, b_{y}`) over assigning in the body — it initializes members directly instead of default-constructing then overwriting. This is RAII (Tier 2.10) realized at the type level.

**Syntax**

```cpp
class T {
public:
    T() = default;                    // explicitly defaulted
    T(int x) : x_{x} {}               // member initializer list
    ~T() { /* release resources */ }  // destructor
private:
    int x_{};
};
```

**Minimal compileable example**

```cpp
#include <iostream>

class Timer {
public:
    explicit Timer(const char* label) : label_{label} {
        std::cout << "[start] " << label_ << '\n';
    }
    ~Timer() {                                  // runs at scope exit
        std::cout << "[stop]  " << label_ << '\n';
    }
private:
    const char* label_;
};

int main() {
    Timer t{"main"};                            // constructed
    std::cout << "working...\n";
    return 0;                                    // ~Timer() runs here
}
// Output: [start] main / working... / [stop]  main
```

**Common pitfalls / Notes**
- ⚠️ Members are initialized in **declaration order**, *not* the order written in the initializer list. Listing them out of order (and depending on it) is a bug `-Wreorder` warns about.
- ⚠️ A constructor that throws means the destructor will **not** run for that object (it was never fully constructed) — but already-constructed members/bases are destroyed.
- ⚠️ If your class is meant to be a polymorphic base (used via base pointers), its destructor must be `virtual` (see 3.7) — otherwise `delete base_ptr` is UB.
- 💡 Use `= default` to ask the compiler for the standard constructor/destructor; use `= delete` to forbid one.

*(Source: https://en.cppreference.com/w/cpp/language/constructor)*

**➤ Try it yourself:** Create two `Timer` objects in the same scope and confirm destruction happens in reverse construction order.

---

<a id="t3-rule5"></a>
## 3.3 Copy / Move Constructors and the Rule of 5

When a class **owns a resource** (heap memory, a file handle), the compiler-generated copy operations do a shallow copy — copying the pointer, not the resource — which leads to double-free and aliasing bugs. The **Rule of 5** says: if you define any one of the destructor, copy constructor, copy assignment, move constructor, or move assignment, you almost always need to consider all five. The **Rule of 0** is the modern ideal: own resources through members that already manage themselves (e.g. `std::vector`, `std::unique_ptr`) so you need *none* of the five.

**Syntax**

```cpp
class T {
public:
    ~T();                          // 1 destructor
    T(const T&);                   // 2 copy constructor
    T& operator=(const T&);        // 3 copy assignment
    T(T&&) noexcept;               // 4 move constructor
    T& operator=(T&&) noexcept;    // 5 move assignment
};
```

**Minimal compileable example** — an owning buffer with all five:

```cpp
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(std::size_t n) : size_{n}, data_{new int[n]{}} {}

    ~Buffer() { delete[] data_; }                       // 1

    Buffer(const Buffer& o) : size_{o.size_}, data_{new int[o.size_]} {  // 2
        std::copy(o.data_, o.data_ + size_, data_);
    }
    Buffer& operator=(const Buffer& o) {                // 3 (copy-and-swap)
        Buffer tmp{o};
        swap(tmp);
        return *this;
    }
    Buffer(Buffer&& o) noexcept                          // 4
        : size_{o.size_}, data_{o.data_} {
        o.size_ = 0; o.data_ = nullptr;                 // leave source empty
    }
    Buffer& operator=(Buffer&& o) noexcept {            // 5
        Buffer tmp{std::move(o)};
        swap(tmp);
        return *this;
    }

    std::size_t size() const { return size_; }
    void swap(Buffer& o) noexcept {
        std::swap(size_, o.size_);
        std::swap(data_, o.data_);
    }
private:
    std::size_t size_;
    int*        data_;
};

int main() {
    Buffer a{4};
    Buffer b = a;              // copy ctor -> independent buffer
    Buffer c = std::move(a);   // move ctor -> steals a's buffer
    std::cout << b.size() << ' ' << c.size() << ' ' << a.size() << '\n'; // 4 4 0
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Defaulted copy on a class holding a raw owning pointer = **double free** (both copies delete the same memory). This is exactly why the Rule of 5 exists.
- ⚠️ Move operations should be `noexcept` — otherwise containers like `std::vector` fall back to copying on reallocation for exception safety.
- ⚠️ Always leave a **moved-from** object in a valid (destructible/assignable) state; here we null it out.
- 💡 **Prefer the Rule of 0:** replace `int* data_` with `std::vector<int>` or `std::unique_ptr<int[]>` and delete all five special members — correctness for free. The example above exists to show what those types do *for* you.

*(Source: https://en.cppreference.com/w/cpp/language/rule_of_three)*

**➤ Try it yourself:** Rewrite `Buffer` to hold a `std::vector<int>` and remove all five special members. Confirm copy and move still work — that's the Rule of 0.

---

<a id="t3-this-static-const"></a>
## 3.4 `this` Pointer, `static` Members, `const` Member Functions

Inside a non-static member function, `this` is a pointer to the object the function was called on. A **`static` member** belongs to the class itself, not to any instance — one shared copy for all objects. A **`const` member function** promises not to modify the object's observable state, so it can be called on `const` objects; mark every member function that doesn't mutate as `const`.

**Syntax**

```cpp
class T {
public:
    int  get() const { return v_; }    // const member function
    T&   set(int v) { v_ = v; return *this; }  // returns *this for chaining
    static int count();                // static member function
private:
    int v_{};
    static int instances_;             // static data member (declaration)
};
int T::instances_ = 0;                 // definition (in a .cpp)
```

**Minimal compileable example**

```cpp
#include <iostream>

class Widget {
public:
    Widget()  { ++count_; }
    ~Widget() { --count_; }

    Widget& name(int n) { id_ = n; return *this; }   // chainable via *this
    int id() const { return id_; }                   // const: read-only

    static int alive() { return count_; }            // class-wide state
private:
    int id_{0};
    static inline int count_{0};   // C++17 inline static: define in-class
};

int main() {
    Widget a; Widget b;
    a.name(1);                      // method chaining uses *this
    const Widget& cb = b;
    std::cout << cb.id() << '\n';   // 0  (const object -> only const methods)
    std::cout << Widget::alive() << '\n';  // 2 (shared static count)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Calling a non-`const` member function on a `const` object is a compile error — hence "const-correctness" must be designed in from the start.
- ⚠️ Pre-C++17, `static` data members needed a separate out-of-class definition. C++17 **`inline` static** members can be initialized in-class (used above).
- ⚠️ `static` member functions have no `this` — they can't access non-static members.
- 💡 Returning `*this` by reference enables fluent **method chaining** (`obj.a().b().c()`).
- 💡 `mutable` lets a specific member change inside a `const` function (e.g. a cache); use sparingly.

*(Source: https://en.cppreference.com/w/cpp/language/member_functions)*

**➤ Try it yourself:** Add a `static int next_id()` that hands out increasing IDs, and have the constructor call it so each `Widget` gets a unique id.

---

<a id="t3-operators"></a>
## 3.5 Operator Overloading

Operator overloading lets your types use natural syntax (`a + b`, `a == b`, `s << x`). Overload operators only when the meaning is **obvious and consistent** with built-in semantics. Symmetric binary operators (like `+`, `==`) are often best as **non-member** functions (frequently `friend`s) so both operands convert uniformly; stream operators (`<<`, `>>`) *must* be non-members because the left operand is the stream.

**Syntax**

```cpp
class T {
public:
    T  operator+(const T& rhs) const;        // member form
    bool operator==(const T& rhs) const;
};
T operator+(const T& a, const T& b);          // non-member form
std::ostream& operator<<(std::ostream&, const T&);  // must be non-member
```

**Minimal compileable example**

```cpp
#include <iostream>

class Vec2 {
public:
    Vec2(double x, double y) : x_{x}, y_{y} {}

    friend Vec2 operator+(const Vec2& a, const Vec2& b) {  // symmetric
        return Vec2{a.x_ + b.x_, a.y_ + b.y_};
    }
    friend bool operator==(const Vec2& a, const Vec2& b) {
        return a.x_ == b.x_ && a.y_ == b.y_;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        return os << '(' << v.x_ << ", " << v.y_ << ')';
    }
private:
    double x_, y_;
};

int main() {
    Vec2 a{1, 2}, b{3, 4};
    std::cout << (a + b) << '\n';        // (4, 6)
    std::cout << (a == b) << '\n';       // 0
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Don't overload operators with surprising semantics (e.g. `operator+` that mutates). Principle of least astonishment.
- ⚠️ Some operators **must** be members: `=`, `[]`, `()`, `->`. Stream operators **must** be non-members.
- ⚠️ Overloading `&&`, `||`, or `,` loses their short-circuit/sequencing behavior — avoid.
- 💡 Implement `+=` first, then define `+` in terms of it (`a + b` → copy then `+=`) to avoid duplicating logic. Likewise define `!=` as `!(a == b)`.
- 💡 *(C++20+ — informational only)* C++20 adds the spaceship operator `<=>` to auto-generate all comparisons; in C++17 you write them yourself.

*(Source: https://en.cppreference.com/w/cpp/language/operators)*

**➤ Try it yourself:** Add `operator-`, `operator+=`, and a scalar `operator*(double)` to `Vec2`, defining `operator+` in terms of `operator+=`.

---

<a id="t3-inheritance"></a>
## 3.6 Inheritance: public / protected / private, `final`, `override`

Inheritance creates a new class from an existing one, reusing and extending it. **`public`** inheritance models "**is-a**" (a `Dog` is an `Animal`) and is by far the most common. `protected`/`private` inheritance model "implemented-in-terms-of" and are rare. Use `override` on functions meant to replace a virtual base function (the compiler verifies you got the signature right), and `final` to forbid further overriding or derivation.

**Syntax**

```cpp
class Base { /* ... */ };
class Derived : public Base { /* is-a Base */ };

class D : public Base {
    void f() override;     // must match a virtual Base::f
};
class Sealed final { };    // cannot be derived from
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(std::string name) : name_{std::move(name)} {}
    virtual ~Animal() = default;                 // virtual base destructor
    virtual std::string sound() const { return "..."; }
    const std::string& name() const { return name_; }
protected:
    std::string name_;                            // accessible to derived
};

class Dog final : public Animal {                 // is-a Animal; not derivable
public:
    using Animal::Animal;                         // inherit constructors
    std::string sound() const override { return "Woof"; }   // checked override
};

int main() {
    Dog d{"Rex"};
    std::cout << d.name() << ": " << d.sound() << '\n';   // Rex: Woof
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Without `override`, a typo'd signature creates a *new* function instead of overriding — a silent bug. Always write `override`.
- ⚠️ `public` inheritance is a strong promise: a `Derived` must be substitutable anywhere a `Base` is expected (Liskov). If it isn't, prefer composition.
- ⚠️ Don't confuse **overriding** (virtual, runtime) with **hiding** (a derived name shadows all base overloads of that name — bring them in with `using Base::f;`).
- 💡 `using Animal::Animal;` inherits the base constructors so you don't re-declare them.

*(Source: https://en.cppreference.com/w/cpp/language/derived_class)*

**➤ Try it yourself:** Add a `Cat` class that overrides `sound()`. Store a `Dog` and a `Cat` and call `sound()` on each (next section makes this polymorphic through a base pointer).

---

<a id="t3-polymorphism"></a>
## 3.7 Polymorphism: Virtual Functions, vtable, Dynamic Dispatch

A **virtual** function is resolved at **runtime** based on the object's actual (dynamic) type, not the static type of the pointer/reference used to call it. The compiler implements this with a per-class **vtable** (a table of function pointers); each polymorphic object holds a hidden `vptr` to its class's vtable. This **dynamic dispatch** is what lets you treat different derived objects uniformly through a base pointer/reference.

```
   Base* p ──▶ [ vptr ]──▶  Dog vtable: { ~Dog, sound=Dog::sound }
                                  │
   p->sound()  looks up sound in the vtable  ──▶ calls Dog::sound at runtime
```

**Syntax**

```cpp
class Base { public: virtual void f(); virtual ~Base() = default; };
class Derived : public Base { public: void f() override; };

Base* p = new Derived;
p->f();          // calls Derived::f (dynamic dispatch)
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <memory>
#include <vector>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;          // pure virtual (see 3.8)
};
struct Circle : Shape {
    explicit Circle(double r) : r_{r} {}
    double area() const override { return 3.14159265 * r_ * r_; }
    double r_;
};
struct Square : Shape {
    explicit Square(double s) : s_{s} {}
    double area() const override { return s_ * s_; }
    double s_;
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;   // base pointers
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Square>(2.0));

    for (const auto& s : shapes)                   // uniform treatment
        std::cout << s->area() << '\n';            // 3.14159 then 4
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **A base class used polymorphically MUST have a `virtual` destructor.** `delete base_ptr` on a non-virtual destructor is undefined behavior (the derived part isn't destroyed).
- ⚠️ **Object slicing:** copying a derived object *into a base value* (`Base b = derived;`) drops the derived part and loses polymorphism. Use pointers/references for polymorphic handling.
- ⚠️ Never call virtual functions from constructors/destructors expecting derived behavior — during base construction the object *is* a base, so the base version runs.
- 💡 Virtual dispatch costs one indirection; for hot inner loops consider non-virtual designs, but don't prematurely optimize.

*(Source: https://en.cppreference.com/w/cpp/language/virtual)*

**➤ Try it yourself:** Add a `Triangle` shape and a free function `double total_area(const std::vector<std::unique_ptr<Shape>>&)` that sums all areas polymorphically.

---

<a id="t3-abstract"></a>
## 3.8 Abstract Classes & Pure Virtual Functions (`= 0`)

A **pure virtual** function (`virtual T f() = 0;`) declares an interface with no (required) implementation, making its class **abstract** — you cannot instantiate it directly, only derive from it and implement the function. Abstract classes define contracts; they're how C++ expresses interfaces. A class with at least one pure virtual function is abstract.

**Syntax**

```cpp
class Interface {
public:
    virtual ~Interface() = default;
    virtual void execute() = 0;     // pure virtual -> abstract class
};
class Impl : public Interface {
public:
    void execute() override { /* ... */ }   // must implement to be concrete
};
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <memory>

class Logger {                          // abstract interface
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& msg) = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& msg) override {
        std::cout << "[console] " << msg << '\n';
    }
};

void run(Logger& sink) { sink.log("hello"); }   // depends on the interface

int main() {
    // Logger l;                 // ⚠️ ERROR: cannot instantiate abstract class
    ConsoleLogger c;
    run(c);                      // [console] hello
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Forgetting to override **all** pure virtuals leaves the derived class abstract too — you'll get "cannot instantiate" errors.
- ⚠️ Still give an abstract base a `virtual ~Base() = default;` — it will be deleted through base pointers.
- 💡 A pure virtual function *may* still have a body (`void f() = 0; ... void Base::f(){}`) to provide a common default that derived classes call explicitly — uncommon but legal.
- 💡 Program to interfaces (abstract bases), not concrete classes — this is the Dependency Inversion Principle (3.11).

*(Source: https://en.cppreference.com/w/cpp/language/abstract_class)*

**➤ Try it yourself:** Add a `FileLogger` implementing `Logger` (it can just print a "would write to file" message) and pass it to `run` through the base reference.

---

<a id="t3-multiple-inheritance"></a>
## 3.9 Multiple Inheritance, Virtual Inheritance, the Diamond Problem

C++ allows a class to inherit from **multiple** bases. When two bases share a common ancestor, the derived object ends up with **two copies** of that ancestor — the **diamond problem** — causing ambiguity. **Virtual inheritance** (`class B : virtual public A`) makes the shared base a single, shared sub-object, resolving the duplication.

```
        A                 A  (single, shared via virtual inheritance)
       / \               / \
      B   C    vs.   (virtual) (virtual)
       \ /               \ /
        D                 D
   two A copies        one A copy
```

**Syntax**

```cpp
class A { public: int v; };
class B : virtual public A {};     // virtual inheritance
class C : virtual public A {};
class D : public B, public C {};   // D has ONE A sub-object
```

**Minimal compileable example**

```cpp
#include <iostream>

struct Device {
    explicit Device(int id) : id_{id} {}
    int id_;
};
struct Scanner  : virtual Device { Scanner()  : Device{0} {} };
struct Printer  : virtual Device { Printer()  : Device{0} {} };

struct Copier : Scanner, Printer {
    Copier(int id) : Device{id} {}     // most-derived initializes virtual base
};

int main() {
    Copier c{42};
    std::cout << c.id_ << '\n';        // 42 — unambiguous: one Device
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ With virtual inheritance, the **most-derived class** (`Copier`) is responsible for constructing the virtual base — the intermediate classes' base initializers are ignored for that shared sub-object.
- ⚠️ Without `virtual`, `c.id_` would be ambiguous (two `Device::id_`); you'd need `c.Scanner::id_`.
- ⚠️ Multiple inheritance of **concrete state** is error-prone and often signals a design smell. Multiple inheritance of **interfaces** (pure-virtual bases) is clean and common.
- 💡 Prefer "inherit one concrete base + several interfaces" over inheriting multiple stateful bases.

*(Source: https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes)*

**➤ Try it yourself:** Remove the `virtual` keywords and observe the ambiguity error when accessing `c.id_`; then restore them.

---

<a id="t3-casts"></a>
## 3.10 The Casts: `dynamic_cast`, `static_cast`, `reinterpret_cast`, `const_cast`

C++ provides four named casts, each with a narrow purpose, replacing the dangerous C-style `(T)x`. **`static_cast`** does compile-time-checked conversions (numeric, up/down a known hierarchy). **`dynamic_cast`** does runtime-checked downcasts in polymorphic hierarchies (returns `nullptr`/throws on failure). **`const_cast`** adds/removes `const`. **`reinterpret_cast`** reinterprets bit patterns — almost always a red flag.

**Syntax**

```cpp
static_cast<T>(expr)        // related types, checked at compile time
dynamic_cast<T*>(expr)      // safe downcast; nullptr if wrong type
const_cast<T>(expr)         // strip/add const (or volatile)
reinterpret_cast<T>(expr)   // low-level bit reinterpretation
```

**Minimal compileable example**

```cpp
#include <iostream>

struct Base { virtual ~Base() = default; };
struct Derived : Base { void special() { std::cout << "special\n"; } };

int main() {
    double d = 3.9;
    int n = static_cast<int>(d);          // 3 (explicit, intentional)
    std::cout << n << '\n';

    Base* b = new Derived;
    if (auto* dp = dynamic_cast<Derived*>(b))   // runtime-checked downcast
        dp->special();                          // "special"
    delete b;

    Base* b2 = new Base;
    std::cout << (dynamic_cast<Derived*>(b2) == nullptr) << '\n';  // 1: fails safely
    delete b2;
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `dynamic_cast` requires a **polymorphic** type (at least one virtual function) and has runtime cost. If you find yourself using it a lot, a virtual function probably belongs in the base instead.
- ⚠️ `const_cast`-ing away `const` and then **writing** to an originally-`const` object is undefined behavior. Only use it to interface with legacy non-const APIs you know don't mutate.
- ⚠️ `reinterpret_cast` results are mostly implementation-defined; dereferencing as an unrelated type usually violates strict aliasing (UB). Avoid unless doing low-level/serialization work you fully understand.
- 💡 Never use C-style casts in C++ — they silently pick whichever of the four (incl. the dangerous ones) compiles, hiding intent.

*(Source: https://en.cppreference.com/w/cpp/language/dynamic_cast)*

**➤ Try it yourself:** Write a function `void poke(Base* b)` that uses `dynamic_cast` to call `special()` only when `b` actually points to a `Derived`.

---

<a id="t3-solid"></a>
## 3.11 SOLID Principles Applied to C++

SOLID is five design guidelines for maintainable OO code. **S**ingle Responsibility (a class has one reason to change). **O**pen/Closed (open for extension, closed for modification — add behavior via new types, not by editing existing ones). **L**iskov Substitution (a derived type must be usable wherever its base is). **I**nterface Segregation (many small interfaces beat one fat one). **D**ependency Inversion (depend on abstractions, not concretions).

**Minimal compileable example** — Open/Closed + Dependency Inversion via an abstract interface:

```cpp
#include <iostream>
#include <memory>
#include <vector>

// DIP: high-level code depends on this abstraction, not concrete sinks.
struct Notifier {
    virtual ~Notifier() = default;
    virtual void send(const std::string& msg) const = 0;
};

struct EmailNotifier : Notifier {
    void send(const std::string& m) const override { std::cout << "email: " << m << '\n'; }
};
struct SmsNotifier : Notifier {                 // OCP: extend by adding a type
    void send(const std::string& m) const override { std::cout << "sms: " << m << '\n'; }
};

// Open for extension (new Notifier types) without modifying this function.
void alert(const std::vector<std::unique_ptr<Notifier>>& sinks, const std::string& m) {
    for (const auto& s : sinks) s->send(m);     // LSP: any Notifier works
}

int main() {
    std::vector<std::unique_ptr<Notifier>> sinks;
    sinks.push_back(std::make_unique<EmailNotifier>());
    sinks.push_back(std::make_unique<SmsNotifier>());
    alert(sinks, "deploy finished");
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **LSP violations** are subtle: the classic `Square : Rectangle` breaks code that sets width/height independently. If a subtype can't honor the base's contract, it isn't an "is-a".
- ⚠️ Over-applying SOLID leads to a maze of tiny classes/interfaces. Apply it where change is likely, not everywhere.
- 💡 In C++, "depend on abstractions" usually means pure-virtual interfaces *or* templates (compile-time polymorphism, Tier 4) — both invert the dependency.
- 💡 Interface Segregation maps directly to small pure-virtual base classes that clients implement only as needed.

*(Source: https://en.cppreference.com/w/cpp/language/abstract_class)*

**➤ Try it yourself:** Add a `SlackNotifier` without touching `alert` or the existing notifiers — that's the Open/Closed Principle in action.

---

<a id="t3-composition"></a>
## 3.12 Composition vs Inheritance

**Inheritance** models "is-a" and couples the derived class tightly to the base's implementation. **Composition** models "has-a"/"uses-a" by holding other objects as members and delegating to them — looser coupling, more flexibility, and no fragile-base-class problems. The modern guideline: **prefer composition over inheritance**, reserving (public) inheritance for genuine substitutable "is-a" relationships and polymorphism.

**Syntax**

```cpp
class Engine { public: void start(); };

class Car {            // composition: Car HAS-A Engine
    Engine engine_;
public:
    void start() { engine_.start(); }   // delegate
};
```

**Minimal compileable example**

```cpp
#include <iostream>

class Engine {
public:
    void start() { std::cout << "engine started\n"; }
};

class Car {                    // HAS-A Engine (composition)
public:
    void drive() {
        engine_.start();       // delegate to the member
        std::cout << "driving\n";
    }
private:
    Engine engine_;            // owned sub-object
};

int main() {
    Car c;
    c.drive();                 // engine started / driving
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Reaching for inheritance just to **reuse code** is the classic mistake — it exposes you to the base's full interface and invariants. Compose and delegate instead.
- ⚠️ Deep inheritance hierarchies are rigid and hard to change; composition lets you swap collaborators (even at runtime, via interface members).
- 💡 Use inheritance when you need **runtime polymorphism** through a base pointer/reference; use composition for code reuse and "has-a" relationships.
- 💡 Composition + an interface member (a `std::unique_ptr<Strategy>`) gives you the Strategy pattern: swap behavior without inheritance.

*(Source: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-domain)*

**➤ Try it yourself:** Give `Car` a `std::unique_ptr<Engine>` instead of a value member, add an abstract `Engine` interface with `ElectricEngine`/`PetrolEngine` implementations, and switch engines at runtime.

---

### ✅ Tier 3 Checkpoint

You can now: design encapsulated classes; implement the Rule of 5 (and prefer the Rule of 0); use `this`, `static`, and `const` correctly; overload operators sensibly; build inheritance hierarchies with `override`/`final`; use virtual functions and understand vtables/dynamic dispatch; define abstract interfaces; navigate multiple/virtual inheritance; choose the right cast; and apply SOLID and composition-over-inheritance.

**Self-test:** Design a small shape hierarchy: an abstract `Shape` with pure-virtual `area()` and `name()`, concrete `Circle`/`Rectangle`, a `virtual` destructor, and a function that takes `std::vector<std::unique_ptr<Shape>>` and prints each shape's name and area. Then refactor one "is-a" that's really a "has-a" into composition.

---

## Next Step

**Tier 3 is complete.** I'll pause here. Tell me if you want any adjustments — otherwise say **go** and I'll append **🔵 Tier 4 — Modern C++17 & Advanced Language** to the end of this file.

=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================

<a id="tier-4"></a>
---

# 🔵 Tier 4 — Modern C++17 & Advanced Language (Senior)

### 4.0 Prerequisites · Learning Goals · ⏱️ Study Time

**Prerequisites (from Tiers 1–3):** value vs reference semantics, RAII, the Rule of 5/0, `const`-correctness, classes and virtual functions, references and pointers, and `std::string`/`std::vector` basics. You should be fluent compiling with `g++ -std=c++17 -Wall -Wextra -pedantic`.

**Learning goals.** After this tier you can:
- Explain lvalues/rvalues and use move semantics and perfect forwarding correctly.
- Manage ownership with `unique_ptr`, `shared_ptr`, and `weak_ptr`.
- Write function and class templates, specializations, and variadic templates with fold expressions.
- Constrain templates with SFINAE / `std::enable_if`.
- Write lambdas with the right capture mode, including generic lambdas.
- Use the C++17 vocabulary types: structured bindings, `optional`, `variant`, `any`, `filesystem`.
- Apply C++17 language features: `if`/`switch` initializers, inline variables, and the standard attributes.
- Write exception-safe code and reason about `noexcept`.

**⏱️ Estimated study time:** ~28–34 hours (about 3–3.5 weeks).

> 💡 This tier is where you stop *using* the language and start *wielding* it. Ownership, lifetime, and performance are the throughline.

---

<a id="t4-move"></a>
## 4.1 Move Semantics, Rvalue References (`&&`), `std::move`, Perfect Forwarding

Every expression is an **lvalue** (has identity, you can take its address) or an **rvalue** (a temporary, safe to "steal" from). An **rvalue reference** `T&&` binds to rvalues, enabling **move semantics**: instead of copying an expensive resource, a moved-from object transfers ownership (e.g. steals a heap pointer) and leaves the source empty. `std::move` is just a cast to `T&&` (it moves nothing itself); **perfect forwarding** with `T&&` + `std::forward` preserves an argument's value category through a template.

**Syntax**

```cpp
void sink(std::string&& s);          // binds to rvalues (temporaries)
auto y = std::move(x);                // cast x to rvalue -> enables move
template<class T> void wrap(T&& a) {  // forwarding (universal) reference
    inner(std::forward<T>(a));        // preserves lvalue/rvalue-ness
}
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::string a = "a very long string that owns heap memory";
    std::vector<std::string> v;

    v.push_back(a);              // COPY: a still usable afterward
    v.push_back(std::move(a));   // MOVE: steals a's buffer, no allocation
    std::cout << "a is now: \"" << a << "\"\n";   // valid but unspecified (likely empty)
    std::cout << v[0] << '\n' << v[1] << '\n';
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `std::move` does **not** move — it's a cast. The move happens when the result binds to a move constructor/assignment. `std::move` on a `const` object silently *copies* (can't steal from const).
- ⚠️ A **moved-from** object is valid but unspecified: you may destroy or assign to it, but don't assume its value.
- ⚠️ `T&&` is a **forwarding reference** *only* in a deduced template context (`template<class T> f(T&&)`). `void f(std::string&&)` is a plain rvalue reference.
- ⚠️ Don't `return std::move(local);` — it disables copy elision (NRVO). Just `return local;`.
- 💡 Use `std::forward<T>` only with forwarding references; use `std::move` for known rvalue references/locals you're done with.

*(Source: https://en.cppreference.com/w/cpp/language/value_category)*

**➤ Try it yourself:** Add print statements to a class's copy and move constructors, then watch which one fires for `push_back(x)` vs `push_back(std::move(x))`.

---

<a id="t4-smart-pointers"></a>
## 4.2 Smart Pointers: `unique_ptr`, `shared_ptr`, `weak_ptr`, `make_*`

Smart pointers (from `<memory>`) automate heap lifetime via RAII so you never write `delete`. **`unique_ptr`** is a zero-overhead, move-only *sole* owner. **`shared_ptr`** is reference-counted *shared* ownership (the object dies when the last owner does). **`weak_ptr`** is a non-owning observer of a `shared_ptr` that breaks reference cycles. Prefer `make_unique`/`make_shared` — they're exception-safe and (for shared) allocate control block + object together.

**Syntax**

```cpp
auto u = std::make_unique<T>(args);     // sole owner, move-only
auto s = std::make_shared<T>(args);     // shared, ref-counted
std::weak_ptr<T> w = s;                 // observe without owning
if (auto locked = w.lock()) { /* use *locked */ }   // promote if alive
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::shared_ptr<Node> next;          // shared ownership of the next node
    std::weak_ptr<Node>   prev;          // weak: avoids a cycle that would leak
    explicit Node(int v) : value{v} {}
};

int main() {
    auto u = std::make_unique<int>(42);  // unique ownership
    std::cout << *u << '\n';             // 42

    auto a = std::make_shared<Node>(1);
    auto b = std::make_shared<Node>(2);
    a->next = b;                         // a shares ownership of b
    b->prev = a;                         // weak -> no cycle, both freed at exit
    std::cout << a->next->value << ' '
              << b->prev.lock()->value << '\n';   // 2 1
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Two `shared_ptr`s pointing at each other form a **reference cycle** that never reaches count 0 — a leak. Break it with `weak_ptr` for the back-edge (as above).
- ⚠️ Never build two independent smart pointers from the same raw pointer (`shared_ptr<T> a{p}, b{p};`) — double free. Always go through `make_*`.
- ⚠️ `shared_ptr` is thread-safe for the *control block* (ref count), **not** for the pointed-to object — you still need synchronization for the data.
- 💡 Default to `unique_ptr`; reach for `shared_ptr` only when ownership is genuinely shared. `unique_ptr` converts to `shared_ptr`, not vice versa.
- 💡 For arrays use `std::make_unique<T[]>(n)` (or just `std::vector`).

*(Source: https://en.cppreference.com/w/cpp/memory)*

**➤ Try it yourself:** Build a 3-node singly linked list with `unique_ptr` and write a function that prints all values by following `next`.

---

<a id="t4-templates"></a>
## 4.3 Templates: Function Templates, Class Templates, Specialization

Templates are compile-time blueprints: you write code parameterized by type (or value), and the compiler stamps out a concrete version for each instantiation. **Function templates** generalize algorithms; **class templates** generalize data structures (every STL container is one). **Specialization** provides a custom implementation for a specific type when the generic one isn't right.

**Syntax**

```cpp
template<class T> T max_of(T a, T b);           // function template
template<class T> class Box { T value_; };      // class template
template<> class Box<bool> { /* custom */ };    // full specialization
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <string>

template<class T>
T max_of(T a, T b) { return (a < b) ? b : a; }   // works for any comparable T

template<class T>
class Box {
public:
    explicit Box(T v) : value_{std::move(v)} {}
    const T& get() const { return value_; }
private:
    T value_;
};

// Full specialization: store bools differently (here, just a marker)
template<>
class Box<bool> {
public:
    explicit Box(bool v) : flag_{v} {}
    const char* get() const { return flag_ ? "true" : "false"; }
private:
    bool flag_;
};

int main() {
    std::cout << max_of(3, 7) << '\n';                 // 7  (T=int)
    std::cout << max_of(std::string{"a"}, std::string{"b"}) << '\n';  // b
    std::cout << Box<int>{42}.get() << '\n';           // 42
    std::cout << Box<bool>{true}.get() << '\n';        // true (specialized)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Template definitions usually must live in **headers** — the compiler needs the full definition at each instantiation point (hence "undefined reference" if you put them in a `.cpp`).
- ⚠️ Errors surface at **instantiation**, often as long messages. Compile with a small test instantiation early.
- ⚠️ **Function templates** support overloading but not partial specialization; **class templates** support partial specialization but you often prefer overloading/`if constexpr` for functions.
- 💡 Use `template<class T>` and `template<typename T>` interchangeably — pick one style.
- 💡 C++17's **CTAD** (class template argument deduction) lets you write `Box b{42};` and have `T` deduced.

*(Source: https://en.cppreference.com/w/cpp/language/templates)*

**➤ Try it yourself:** Write a `template<class T> void print_all(const std::vector<T>&)` and call it for `int` and `std::string` vectors.

---

<a id="t4-variadic"></a>
## 4.4 Variadic Templates & Fold Expressions (C++17)

A **variadic template** accepts any number of template arguments via a *parameter pack* (`class... Ts`). Before C++17 you processed packs with recursion; **fold expressions** (C++17) collapse a pack over a binary operator in one expression — `(args + ...)`, `(std::cout << ... << args)` — making variadic code concise and efficient.

**Syntax**

```cpp
template<class... Ts> void f(Ts... args);   // parameter pack
(args + ...)        // unary right fold:  a1 + (a2 + (a3 + ...))
(... + args)        // unary left fold:   ((a1 + a2) + a3) + ...
(args + ... + 0)    // binary fold with init value (safe for empty pack)
```

**Minimal compileable example**

```cpp
#include <iostream>

template<class... Ts>
auto sum(Ts... args) {
    return (args + ... + 0);          // C++17 fold; 0 handles empty pack
}

template<class... Ts>
void print(const Ts&... args) {
    ((std::cout << args << ' '), ...);   // fold over the comma operator
    std::cout << '\n';
}

int main() {
    std::cout << sum(1, 2, 3, 4) << '\n';   // 10
    std::cout << sum(1.5, 2.5) << '\n';     // 4
    print("x", 42, 3.14, 'c');              // x 42 3.14 c
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `sizeof...(args)` gives the **count** of pack elements (note the `...`), distinct from `sizeof`.
- ⚠️ A unary fold over an **empty** pack is only valid for `&&` (true), `||` (false), and `,` (void). For `+`, `*`, etc., use a **binary** fold with an identity (`... + 0`) or you'll get a compile error on empty packs.
- ⚠️ Pack expansion `args...` must appear in a context that allows it (function call args, initializer lists, fold expressions).
- 💡 Fold expressions replace most of the old recursive "head + recurse on tail" variadic patterns — reach for them first.

*(Source: https://en.cppreference.com/w/cpp/language/fold)*

**➤ Try it yourself:** Write `template<class... Ts> bool all_true(Ts... xs)` using a `(... && xs)` fold, and test it with several `bool`s.

---

<a id="t4-sfinae"></a>
## 4.5 SFINAE Basics & `std::enable_if`

**SFINAE** — "Substitution Failure Is Not An Error" — means that when the compiler substitutes template arguments and a *substitution* produces an invalid type/expression, that overload is silently removed from consideration instead of causing a hard error. `std::enable_if` exploits this to **conditionally enable** a template only when a trait holds, letting you write overloads selected by type properties.

**Syntax**

```cpp
// Enabled only when Cond is true; otherwise this overload disappears.
template<class T, std::enable_if_t<Cond, int> = 0>
ret_type f(T x);
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <type_traits>

// Enabled only for integral types
template<class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void describe(T) { std::cout << "integral\n"; }

// Enabled only for floating-point types
template<class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
void describe(T) { std::cout << "floating point\n"; }

int main() {
    describe(42);       // integral
    describe(3.14);     // floating point
    // describe("hi"); // no matching overload -> compile error (by design)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ SFINAE only applies to failures in the **immediate context** of substitution. An error deeper inside a function body is a hard error, not a removed overload.
- ⚠️ `enable_if` syntax is notoriously verbose and error messages are dense. In C++17 a cleaner alternative for *functions* is often **`if constexpr`** (compile-time branching) inside one template.
- 💡 C++17 adds `_v` trait variable templates (`std::is_integral_v<T>`) and `_t` alias templates (`std::enable_if_t`) — use them instead of `::value`/`::type`.
- 💡 *(C++20+ — informational only)* C++20 **concepts** replace most SFINAE with readable constraints (`requires`); see 4.6.

*(Source: https://en.cppreference.com/w/cpp/types/enable_if)*

**➤ Try it yourself:** Write a single `to_double` template that uses `if constexpr (std::is_integral_v<T>)` to convert, and compare it to the `enable_if` two-overload approach.

---

<a id="t4-concepts"></a>
## 4.6 Concepts Intro *(C++20+ — informational only)*

> ⚠️ **This entire section is C++20+ and is NOT part of core C++17.** It is included only to show where the language is heading. None of this compiles under `-std=c++17`.

**Concepts** are named, reusable compile-time predicates on template parameters. They replace the verbose, hard-to-read SFINAE/`enable_if` machinery with declarative constraints, and they produce dramatically clearer error messages ("`T` does not satisfy `Integral`" instead of a substitution wall).

**Syntax (C++20)**

```cpp
template<class T>
concept Number = std::is_arithmetic_v<T>;          // a named constraint

template<Number T>                                  // constrained template
T twice(T x) { return x + x; }

void f(Number auto x);                              // abbreviated form
```

**Illustrative example (C++20 — will NOT compile with `-std=c++17`)**

```cpp
#include <concepts>      // C++20 header
#include <iostream>

template<std::integral T>            // constrained: only integer types
T square(T x) { return x * x; }

int main() {
    std::cout << square(5) << '\n';  // 25
    // square(3.14);  // clear error: 3.14 does not satisfy std::integral
}
```

**Common pitfalls / Notes**
- ⚠️ Requires `-std=c++20` (or later) and a recent compiler/standard library. Do not use in a C++17 codebase.
- 💡 The **C++17 equivalent today** is `std::enable_if` (4.5) and `if constexpr` (4.11). Mentally map "concept" → "an `enable_if` condition with a nice name".
- 💡 When you eventually migrate to C++20, most `enable_if` overload sets collapse into a single constrained template.

*(Source: https://en.cppreference.com/w/cpp/language/constraints)*

**➤ Try it yourself (conceptual):** Take your `enable_if` `describe` from 4.5 and sketch how it would look as two `std::integral`/`std::floating_point` constrained overloads in C++20.

---

<a id="t4-lambdas"></a>
## 4.7 Lambda Expressions: Capture Modes, Generic Lambdas

A lambda is an inline, anonymous function object. The **capture list** `[...]` controls how it accesses surrounding variables: `[x]` by copy, `[&x]` by reference, `[=]`/`[&]` capture everything used (avoid the catch-alls in non-trivial code), `[this]` captures the enclosing object. **Generic lambdas** use `auto` parameters to become callable templates. Lambdas are the idiomatic way to pass behavior to algorithms (Tier 5).

**Syntax**

```cpp
[capture](params) -> ret { body };     // full form (return type optional)
[x, &y](int n) { return x + y + n; };  // x by copy, y by reference
[](auto a, auto b) { return a + b; };  // generic lambda
[val = expr]() { ... };                // init-capture (C++14+)
```

**Minimal compileable example**

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int threshold = 3;
    std::vector<int> v{1, 2, 3, 4, 5};

    // capture threshold by copy; count elements above it
    auto n = std::count_if(v.begin(), v.end(),
                           [threshold](int x) { return x > threshold; });
    std::cout << n << '\n';     // 2

    auto add = [](auto a, auto b) { return a + b; };   // generic lambda
    std::cout << add(2, 3) << ' ' << add(1.5, 2.5) << '\n';  // 5 4

    int counter = 0;
    auto bump = [&counter]() { ++counter; };           // capture by reference
    bump(); bump();
    std::cout << counter << '\n';   // 2
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **Dangling captures:** a lambda that captures by reference (`[&]`) must not outlive what it refers to — storing such a lambda (e.g. in `std::function`) past the captured object's lifetime is UB.
- ⚠️ `[=]` on a member function captures `this` (a pointer), not a copy of the object — easy to dangle. Be explicit: `[this]` or capture needed members by value via init-capture `[x = x_]`.
- ⚠️ A lambda that needs to modify a by-copy capture must be marked `mutable`.
- 💡 Captureless lambdas convert to plain function pointers; capturing ones don't (store in `std::function` or pass as a template parameter — see 2.8).
- 💡 Init-capture `[p = std::move(ptr)]` lets you move-capture (e.g. a `unique_ptr`) into a lambda.

*(Source: https://en.cppreference.com/w/cpp/language/lambda)*

**➤ Try it yourself:** Use `std::sort` with a lambda comparator to sort a `std::vector<std::string>` by length, then by alphabetical order as a tiebreak.

---

<a id="t4-tuple-bindings"></a>
## 4.8 `std::tuple`, `std::pair`, Structured Bindings (C++17)

`std::pair` holds two values; `std::tuple` holds a fixed sequence of any number of typed values — handy for returning multiple results. **Structured bindings** (C++17) let you unpack a pair/tuple/struct/array into named variables in one line, replacing clunky `std::get<>`/`.first`/`.second` access.

**Syntax**

```cpp
auto [a, b]    = std::pair{1, 2.0};        // unpack a pair
auto [x, y, z] = std::make_tuple(1, 'c', 3.5);   // unpack a tuple
auto& [m, n]   = some_struct;              // bind by reference
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <map>
#include <string>
#include <tuple>

std::tuple<int, std::string, double> make_record() {
    return {1, "Ada", 99.5};
}

int main() {
    auto [id, name, score] = make_record();      // structured binding
    std::cout << id << ' ' << name << ' ' << score << '\n';   // 1 Ada 99.5

    std::map<std::string, int> ages{{"Bob", 30}, {"Eve", 25}};
    for (const auto& [key, value] : ages)        // unpack each map element
        std::cout << key << "=" << value << ' ';
    std::cout << '\n';                            // Bob=30 Eve=25
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Structured bindings introduce **new names**; you can't bind to existing variables, and you can't (currently) `std::tie`-style ignore individual fields by name — use `std::tie` with `std::ignore` if you need to skip.
- ⚠️ `auto [a,b] = ...` copies; use `auto& [a,b] = ...` to bind references and avoid copies / allow mutation.
- ⚠️ The number of names must exactly match the number of members/elements.
- 💡 Structured bindings work on tuples, pairs, arrays, and **any struct with all-public non-static data members** — great for lightweight aggregates.
- 💡 Returning a `tuple` + structured binding is often cleaner than out-parameters; for "value or nothing" prefer `std::optional` (4.9).

*(Source: https://en.cppreference.com/w/cpp/language/structured_binding)*

**➤ Try it yourself:** Write a function returning `std::pair<int,int>` (min and max of a vector) and unpack it with `auto [lo, hi] = ...`.

---

<a id="t4-vocab-types"></a>
## 4.9 `std::optional`, `std::variant`, `std::any` (C++17)

These three C++17 "vocabulary types" model common situations safely. **`std::optional<T>`** holds either a `T` or nothing — the type-safe replacement for "magic" sentinel values or null pointers as "no result". **`std::variant<A,B,...>`** is a type-safe tagged union holding exactly one of several types. **`std::any`** holds a single value of *any* type (retrieved with `std::any_cast`) — use sparingly, when types truly aren't known at compile time.

**Syntax**

```cpp
std::optional<int> o = 5;  o.has_value(); *o; o.value_or(-1);
std::variant<int, std::string> v = "hi"; std::get<std::string>(v);
std::any a = 42; std::any_cast<int>(a);
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <optional>
#include <string>
#include <variant>

std::optional<int> parse_positive(int x) {
    if (x > 0) return x;          // a value
    return std::nullopt;          // "nothing"
}

int main() {
    if (auto r = parse_positive(7))                 // contextually bool
        std::cout << "got " << *r << '\n';          // got 7
    std::cout << parse_positive(-1).value_or(0) << '\n';   // 0

    std::variant<int, std::string> v = std::string{"text"};
    std::visit([](const auto& x){ std::cout << x << '\n'; }, v);  // text
    std::cout << std::holds_alternative<std::string>(v) << '\n'; // 1
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Dereferencing an empty `optional` with `*o` or `o.value()` is UB / throws (`value()` throws `bad_optional_access`). Check first or use `value_or`.
- ⚠️ `std::get<T>(variant)` throws `bad_variant_access` if the variant doesn't currently hold `T`. Use `std::holds_alternative` or `std::visit`.
- ⚠️ `std::any_cast<T>` throws (pointer form returns `nullptr`) on type mismatch; `any` also typically heap-allocates. Prefer `variant` when the set of types is known.
- 💡 Prefer `optional<T>` over `T*`/sentinels for "maybe a value"; prefer `variant` over inheritance when you have a **closed** set of alternatives and want value semantics.
- 💡 `std::visit` with a generic lambda is the clean way to handle every alternative of a variant.

*(Source: https://en.cppreference.com/w/cpp/utility/optional)*

**➤ Try it yourself:** Model a `Result` as `std::variant<int, std::string>` (value or error message) and write a `std::visit` that prints either "ok: N" or "err: msg".

---

<a id="t4-filesystem"></a>
## 4.10 `std::filesystem` (C++17)

`<filesystem>` (namespace `std::filesystem`, commonly aliased `fs`) provides a portable API for paths, files, and directories — replacing platform-specific calls. It handles path manipulation, existence/size queries, iteration, and create/copy/remove operations, with both throwing and `error_code` overloads.

**Syntax**

```cpp
#include <filesystem>
namespace fs = std::filesystem;
fs::path p = "dir/file.txt";
fs::exists(p); fs::file_size(p); fs::create_directories(p.parent_path());
for (const auto& e : fs::directory_iterator(".")) { /* e.path() */ }
```

**Minimal compileable example**

```cpp
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

int main() {
    fs::path p = "example_dir/notes.txt";
    std::cout << "filename:  " << p.filename()  << '\n';   // "notes.txt"
    std::cout << "extension: " << p.extension() << '\n';   // ".txt"
    std::cout << "parent:    " << p.parent_path() << '\n'; // "example_dir"

    std::error_code ec;                          // non-throwing overload
    bool there = fs::exists(p, ec);
    std::cout << "exists: " << there << '\n';     // 0 if it doesn't exist
    return 0;
}
```

```bash
# Some older toolchains need to link the fs library explicitly:
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o app          # modern GCC/Clang
# g++ ... -lstdc++fs    # GCC < 9
```

**Common pitfalls / Notes**
- ⚠️ Many operations have **two forms**: one that throws `filesystem_error` and one taking an `std::error_code&` that reports failures without exceptions. Choose deliberately.
- ⚠️ Filesystem state is racy: a path that `exists()` may be gone a line later (TOCTOU). For real operations, attempt and handle the error rather than pre-checking.
- ⚠️ On GCC versions before 9 you must link `-lstdc++fs` (and `-lc++fs` on older libc++).
- 💡 `fs::path` overloads `/` for joining: `dir / "sub" / "file.txt"` builds paths portably.

*(Source: https://en.cppreference.com/w/cpp/filesystem)*

**➤ Try it yourself:** Iterate over the current directory with `fs::directory_iterator` and print each entry's name and whether it's a regular file.

---

<a id="t4-init-if"></a>
## 4.11 `if` / `switch` with Initializer + `if constexpr` (C++17)

C++17 lets you declare a variable directly in an `if`/`switch` header — `if (init; condition)` — scoping it to the statement and keeping helpers out of the surrounding scope. Separately, **`if constexpr`** is a *compile-time* branch: the false branch is discarded (not compiled), enabling clean, type-dependent template code without SFINAE.

**Syntax**

```cpp
if (auto it = m.find(k); it != m.end()) { /* use it */ }   // init + cond
switch (int v = compute(); v) { case 1: ...; }

template<class T> void f(T x) {
    if constexpr (std::is_integral_v<T>) { /* int path */ }
    else                                 { /* other path */ }
}
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <map>
#include <string>
#include <type_traits>

template<class T>
std::string kind(T) {
    if constexpr (std::is_integral_v<T>) return "integral";
    else if constexpr (std::is_floating_point_v<T>) return "floating";
    else return "other";
}

int main() {
    std::map<std::string,int> m{{"a",1}};
    if (auto it = m.find("a"); it != m.end())     // init-statement
        std::cout << "found " << it->second << '\n';   // found 1

    std::cout << kind(10) << ' ' << kind(2.5) << ' ' << kind("x") << '\n';
    // integral floating other
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ The init-statement's variable is scoped to the entire `if`/`else` (or `switch`) — it's destroyed at the statement's end, which is usually exactly what you want.
- ⚠️ `if constexpr` only discards branches inside a **template**; in a non-template both branches must compile. The discarded branch is not instantiated, so it may contain code invalid for the other type.
- 💡 `if (init; cond)` shines with `map::find`, `lock` acquisition, and any "compute then test" pattern, keeping the temporary out of the outer scope.
- 💡 `if constexpr` replaces many SFINAE/tag-dispatch tricks with straight-line, readable code.

*(Source: https://en.cppreference.com/w/cpp/language/if)*

**➤ Try it yourself:** Rewrite a `find`-then-check on a `std::map` to use the `if (init; cond)` form, and write a `serialize` template that uses `if constexpr` to handle `bool` differently from other types.

---

<a id="t4-inline-var"></a>
## 4.12 Inline Variables (C++17)

Before C++17, defining a variable in a header that was included by multiple translation units caused multiple-definition linker errors; you needed a `.cpp` definition. **`inline` variables** (C++17) may be defined in a header and included everywhere — the linker merges them into one. This is what makes header-only libraries and in-class `static` data member initialization clean.

**Syntax**

```cpp
// in a header, safe to include from many .cpp files:
inline constexpr double kPi = 3.14159265358979;

struct Config {
    static inline int instances = 0;   // in-class definition, no .cpp needed
};
```

**Minimal compileable example**

```cpp
// constants.h
#pragma once
inline constexpr int kMaxUsers = 100;     // one definition across all TUs

struct Counter {
    static inline int total = 0;          // C++17 inline static member
    Counter() { ++total; }
};
```

```cpp
// main.cpp
#include <iostream>
#include "constants.h"

int main() {
    Counter a, b, c;
    std::cout << kMaxUsers << '\n';        // 100
    std::cout << Counter::total << '\n';   // 3
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `constexpr` at namespace scope is already implicitly `inline` for variables in C++17, but writing `inline constexpr` is explicit and clear; non-constexpr header constants need `inline` to avoid ODR violations.
- ⚠️ Without `inline`, a plain `int x = 5;` at namespace scope in a header included by two `.cpp`s is a **multiple definition** linker error.
- 💡 Inline variables removed the long-standing chore of pairing every `static` data member with an out-of-line definition (see 3.4).
- 💡 This feature is the backbone of modern **header-only** libraries.

*(Source: https://en.cppreference.com/w/cpp/language/inline)*

**➤ Try it yourself:** Put an `inline constexpr` config constant in a header, include it from two separate `.cpp` files compiled together, and confirm it links without "multiple definition" errors.

---

<a id="t4-attributes"></a>
## 4.13 Attributes: `[[nodiscard]]`, `[[maybe_unused]]`, `[[fallthrough]]`

Standard attributes give the compiler hints/intent in a portable `[[...]]` syntax. **`[[nodiscard]]`** warns if a function's return value is ignored (great for error codes, `optional`, allocations). **`[[maybe_unused]]`** silences "unused" warnings for entities used only conditionally (e.g. in `assert`). **`[[fallthrough]]`** documents an intentional `switch` fall-through, suppressing the warning.

**Syntax**

```cpp
[[nodiscard]] int compute();            // warn if result ignored
void f([[maybe_unused]] int debugId);   // ok if unused
switch (n) { case 1: do_a(); [[fallthrough]]; case 2: do_b(); }
```

**Minimal compileable example**

```cpp
#include <iostream>

[[nodiscard]] int risky() { return 42; }      // caller must use the result

void handle(int n) {
    switch (n) {
        case 1:
            std::cout << "one ";
            [[fallthrough]];                   // intentional: no warning
        case 2:
            std::cout << "two\n";
            break;
        default:
            std::cout << "other\n";
    }
}

int main() {
    [[maybe_unused]] int unused = 5;           // no -Wunused warning
    int r = risky();                           // good: result used
    std::cout << r << '\n';
    // risky();   // ⚠️ -Wunused-result: discarding [[nodiscard]] value
    handle(1);                                  // one two
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Attributes are **advisory**: an unknown attribute is ignored (compilers may warn). They change diagnostics, not program semantics.
- ⚠️ `[[fallthrough]];` must be a statement immediately before the next `case` label; misplacing it is an error.
- 💡 Mark factory functions, `optional`/`expected` returns, and error-code returns `[[nodiscard]]` — it catches a whole class of "forgot to check" bugs at compile time.
- 💡 *(C++20+ — informational only)* C++20 lets you add a message: `[[nodiscard("check the error")]]`.

*(Source: https://en.cppreference.com/w/cpp/language/attributes)*

**➤ Try it yourself:** Mark a function returning `std::optional<int>` as `[[nodiscard]]`, then call it without using the result and read the compiler warning.

---

<a id="t4-exceptions"></a>
## 4.14 Exceptions: `try` / `catch` / `throw`, Custom Exceptions, `noexcept`

Exceptions report errors that can't be handled locally by unwinding the stack to a matching `catch`, running destructors (RAII!) along the way. `throw` raises an exception; `try`/`catch` handles it. Define custom exceptions by deriving from `std::exception` (usually `std::runtime_error`/`std::logic_error`). **`noexcept`** declares a function won't throw — enabling optimizations and required for move operations to be used by containers.

**Syntax**

```cpp
try { mayThrow(); }
catch (const std::runtime_error& e) { /* specific */ }
catch (const std::exception& e)     { e.what(); }   // base: catch by const ref
catch (...)                          { /* anything */ }

void f() noexcept;                    // promises not to throw
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class InsufficientFunds : public std::runtime_error {
public:
    explicit InsufficientFunds(int needed)
        : std::runtime_error{"need " + std::to_string(needed) + " more"} {}
};

int withdraw(int balance, int amount) {
    if (amount > balance) throw InsufficientFunds{amount - balance};
    return balance - amount;
}

int main() {
    try {
        std::cout << withdraw(100, 30) << '\n';     // 70
        std::cout << withdraw(50, 200) << '\n';     // throws
    } catch (const std::exception& e) {             // catch by const ref to base
        std::cout << "error: " << e.what() << '\n'; // error: need 150 more
    }
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **Always catch by `const&`** (`const std::exception&`) — catching by value slices derived exceptions down to the base type.
- ⚠️ **Never let a destructor throw.** If a destructor throws during stack unwinding from another exception, `std::terminate` is called. Destructors are implicitly `noexcept`.
- ⚠️ A `noexcept` function that *does* throw calls `std::terminate` immediately — only mark functions `noexcept` when you're sure (move ops, swaps, simple getters).
- ⚠️ Don't use exceptions for ordinary control flow; they're for exceptional, non-local error conditions. For expected "no value", prefer `std::optional`/`std::variant` (4.9).
- 💡 RAII is what makes exceptions safe: resources held in stack objects are released automatically as the stack unwinds — no `finally` needed.

*(Source: https://en.cppreference.com/w/cpp/language/try_catch)*

**➤ Try it yourself:** Add a second custom exception (`InvalidAmount` for negative withdrawals), throw the right one, and handle both with separate `catch` clauses ordered most-derived first.

---

### ✅ Tier 4 Checkpoint

You can now: reason about value categories and apply move semantics and perfect forwarding; manage ownership with `unique_ptr`/`shared_ptr`/`weak_ptr`; write function/class/variadic templates with fold expressions and specialization; constrain templates with SFINAE/`if constexpr`; write lambdas with correct captures; use `optional`/`variant`/`any`, structured bindings, and `filesystem`; apply C++17 language features (init-statements, inline variables, attributes); and write exception-safe code with `noexcept`.

**Self-test:** Write a `template<class T> class ScopedResource` that owns a `T` via `std::unique_ptr`, is move-only (Rule of 5 reduced to Rule of 0 by using the smart pointer), exposes a `[[nodiscard]] std::optional<T> release()`, and is constructed via a factory that may `throw` a custom exception. Use a structured binding to consume the result.

---

## Next Step

**Tier 4 is complete.** I'll pause here. Tell me if you want any adjustments — otherwise say **go** and I'll append the 
final **🔴 Tier 5 — STL Mastery & Professional Toolkit** (plus the Cheat Sheet, 12-week plan, 100 interview questions, 
and Further Reading) to the end of this file.

=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================

<a id="tier-5"></a>
---

# 🔴 Tier 5 — STL Mastery & Professional Toolkit (Expert)

### 5.0 Prerequisites · Learning Goals · ⏱️ Study Time

**Prerequisites (from Tiers 1–4):** templates and lambdas, move semantics, smart pointers, iterators-as-pointers intuition, `const`-correctness, RAII, and exceptions. You should be able to read template-heavy code without flinching.

**Learning goals.** After this tier you can:
- Choose the right container by its complexity and memory trade-offs.
- Understand the iterator hierarchy and what each algorithm requires.
- Use `<algorithm>` and `<numeric>` fluently instead of hand-written loops.
- Use function objects, `std::bind`, and `std::invoke`.
- Write correct multithreaded code with mutexes, locks, and condition variables.
- Use atomics and reason (at least informally) about memory ordering.
- Run asynchronous work with `std::async`/`future`/`packaged_task`.
- Apply parallel STL, type traits, `constexpr`, and the surrounding tooling (sanitizers, CMake, package managers).

**⏱️ Estimated study time:** ~40–50 hours (about 4–5 weeks).

> 💡 The skill here is **selection and measurement**: knowing which tool is asymptotically and practically right, then verifying with a benchmark and a sanitizer.

---

<a id="t5-sequence"></a>
## 5.1 Sequence Containers: `vector`, `deque`, `list`, `forward_list`, `array`

Sequence containers store elements in a linear order you control. **`vector`** is a growable contiguous array — the default choice. **`array`** is a fixed-size stack array with a container interface. **`deque`** allows fast push/pop at both ends. **`list`**/**`forward_list`** are (doubly/singly) linked lists with O(1) splicing but poor cache behavior. Reach for `vector` unless a specific access pattern demands otherwise.

| Container | Memory | Random access | Insert/erase middle | Insert ends | Iterator/ref stability |
|---|---|---|---|---|---|
| `array<T,N>` | contiguous, fixed | O(1) | n/a (fixed) | n/a | always stable |
| `vector` | contiguous | O(1) | O(n) | O(1) amortized (back) | invalidated on realloc/erase |
| `deque` | chunked | O(1) | O(n) | O(1) front & back | refs stable on end-insert; iters not |
| `list` | nodes | O(n) | O(1) (with iter) | O(1) | stable except erased element |
| `forward_list` | nodes | O(n) | O(1) (after iter) | O(1) front | stable except erased element |

**Syntax**

```cpp
#include <array>
#include <vector>
#include <deque>
#include <list>
std::array<int,3> a{1,2,3};
std::vector<int>  v{1,2,3};   v.push_back(4);
std::deque<int>   d;          d.push_front(0);
std::list<int>    l{1,2,3};   l.splice(l.begin(), l);
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    v.reserve(4);                     // avoid reallocations
    for (int i = 0; i < 4; ++i) v.push_back(i * i);
    for (int x : v) std::cout << x << ' ';   // 0 1 4 9
    std::cout << "\nsize=" << v.size()
              << " cap=" << v.capacity() << '\n';
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ **`vector` reallocation invalidates** all iterators, pointers, and references to its elements. Don't hold a pointer into a vector across a `push_back` that may grow it. `reserve()` ahead of time when the size is known.
- ⚠️ `list`/`forward_list` waste memory on node pointers and thrash the cache — they're rarely faster than `vector` even for "lots of insertions" in practice. Measure.
- ⚠️ `std::array` size is part of its type; it doesn't decay and `.size()` is `constexpr`.
- 💡 **When to use:** `vector` by default; `array` for fixed small sizes; `deque` for queue-like front+back ops; `list` only when you need O(1) splice and stable iterators.

*(Source: https://en.cppreference.com/w/cpp/container)*

**➤ Try it yourself:** Fill a `vector` with 1M ints and a `list` with the same; time summing them. Note how much faster contiguous memory is.

---

<a id="t5-associative"></a>
## 5.2 Associative Containers: `set`, `map`, `multiset`, `multimap`

Ordered associative containers keep elements **sorted** by key (typically a red-black tree), giving O(log n) lookup/insert/erase and ordered iteration. **`set`** stores unique keys; **`map`** stores unique key→value pairs; the `multi*` variants allow duplicate keys. Use them when you need sorted order or range queries; otherwise the unordered (hash) versions (5.3) are faster.

**Syntax**

```cpp
#include <map>
#include <set>
std::set<int> s{3,1,2};                       // stored sorted: 1,2,3
std::map<std::string,int> m{{"a",1},{"b",2}};
m["c"] = 3;                                    // insert or update
auto it = m.find("a");                         // O(log n)
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <map>

int main() {
    std::map<std::string, int> freq;
    for (std::string w : {"a","b","a","c","a","b"})
        ++freq[w];                       // operator[] default-constructs to 0

    for (const auto& [word, count] : freq)   // iterates in key order
        std::cout << word << ": " << count << '\n';  // a:3 b:2 c:1
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `map::operator[]` **inserts** a default-constructed value if the key is missing — handy for counters, but a silent bug if you only meant to read. Use `.at()` (throws) or `.find()` to read without inserting.
- ⚠️ The key type needs a strict-weak `operator<` (or a custom comparator). Inconsistent comparators cause UB.
- ⚠️ Modifying a key in place (via `const_cast`) breaks the ordering invariant — keys are `const` for a reason.
- 💡 C++17 adds `try_emplace` (no construction if key exists) and `insert_or_assign` (clear update semantics) — prefer them over `[]` when intent matters.
- 💡 C++17 node handles (`extract`/`merge`) let you move nodes between trees without reallocating.

*(Source: https://en.cppreference.com/w/cpp/container/map)*

**➤ Try it yourself:** Build a `std::map<int, std::string>` and use `lower_bound`/`upper_bound` to print all entries with keys in `[10, 20]`.

---

<a id="t5-unordered"></a>
## 5.3 Unordered Containers: `unordered_set`, `unordered_map`, Hash Policies

Unordered containers are **hash tables**: average O(1) lookup/insert/erase, but no ordering and worst-case O(n) on bad hashing/collisions. Keys need a hash function (`std::hash` specialization) and equality. They beat ordered containers when you don't need sorted iteration — which is most of the time for pure lookups.

**Syntax**

```cpp
#include <unordered_map>
std::unordered_map<std::string,int> m;
m.reserve(1000);                 // pre-size to reduce rehashing
m.max_load_factor(0.7f);         // tune collisions vs memory
m["x"] = 1;
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

struct Point { int x, y; bool operator==(const Point& o) const { return x==o.x && y==o.y; } };

struct PointHash {                          // custom hash for a user type
    std::size_t operator()(const Point& p) const noexcept {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

int main() {
    std::unordered_map<Point,std::string,PointHash> labels;
    labels[{1,2}] = "origin-ish";
    std::cout << labels.at({1,2}) << '\n';   // origin-ish
    std::cout << labels.bucket_count() << " buckets\n";
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A poor hash (many collisions) degrades to O(n). For custom types, combine field hashes well (the XOR-shift above is a minimal example; prefer a proper mix like boost's `hash_combine`).
- ⚠️ Insertion can trigger a **rehash**, invalidating iterators (references to *elements* stay valid). `reserve()` to avoid mid-loop rehashing.
- ⚠️ Iteration order is unspecified and may change across runs/insertions — never rely on it.
- 💡 **When to use:** unordered for pure membership/lookup; ordered (`map`/`set`) when you need sorted traversal, range queries, or `lower_bound`.
- 💡 `max_load_factor` trades memory for fewer collisions; lower it for speed, raise it to save memory.

*(Source: https://en.cppreference.com/w/cpp/container/unordered_map)*

**➤ Try it yourself:** Specialize `std::hash<Point>` instead of passing a functor, so you can write `std::unordered_map<Point,std::string>` without the third template argument.

---

<a id="t5-iterators"></a>
## 5.4 Iterator Categories (input/output/forward/bidirectional/random-access)

Iterators generalize pointers; algorithms state their requirements in terms of **iterator categories**, each adding capabilities. **Input** (read once, forward), **Output** (write once, forward), **Forward** (multi-pass read/write), **Bidirectional** (`--` too, e.g. `list`/`map`), **Random-access** (`+ n`, `[]`, comparisons, e.g. `vector`/`deque`/`array`). An algorithm works on any iterator at or above its required category.

```
input  ─┐
output ─┤
        forward ──▶ bidirectional ──▶ random-access
 (more capable to the right; vector = random-access, list = bidirectional)
```

**Syntax**

```cpp
for (auto it = c.begin(); it != c.end(); ++it) { *it; }
auto d = std::distance(first, last);     // O(1) random-access, O(n) otherwise
std::advance(it, 3);                      // moves it forward by 3
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::vector<int> v{10,20,30,40};
    auto it = v.begin();
    it += 2;                                   // random-access: jump
    std::cout << *it << '\n';                  // 30

    std::list<int> l{1,2,3};
    auto lit = l.begin();
    std::advance(lit, 2);                      // works for bidirectional too
    std::cout << *lit << '\n';                 // 3

    std::cout << std::distance(v.begin(), v.end()) << '\n';  // 4
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Only random-access iterators support `it + n`, `it[n]`, `it1 < it2`. For others, use `std::advance`/`std::next`/`std::distance` (which adapt to the category).
- ⚠️ Input iterators are **single-pass** — don't iterate twice or cache them (e.g. stream iterators).
- ⚠️ Mismatching an algorithm's requirement (e.g. `std::sort` needs random-access) is a compile error — `std::sort` won't work on `std::list`; use `list::sort()`.
- 💡 Prefer `std::next(it, n)`/`std::prev(it)` for clarity; they don't mutate `it` like `advance` does.
- 💡 *(C++20+ — informational only)* C++20 replaces tag-based categories with iterator *concepts* and adds contiguous iterators.

*(Source: https://en.cppreference.com/w/cpp/iterator)*

**➤ Try it yourself:** Try to `std::sort` a `std::list` (observe the error), then sort it correctly with the member `l.sort()`.

---

<a id="t5-allocators"></a>
## 5.5 Allocators (`std::allocator`, custom allocators)

An **allocator** is the policy object a container uses to obtain and release raw memory, decoupling "where memory comes from" from "how the container uses it". The default `std::allocator<T>` wraps `::operator new`/`delete`. Custom allocators (memory pools, arenas, stack allocators) can dramatically cut allocation overhead and fragmentation in hot paths.

**Syntax**

```cpp
std::allocator<int> a;
int* p = a.allocate(3);          // raw, uninitialized storage for 3 ints
std::construct_at(p, 42);        // (C++20) or: a.construct(p, 42) pre-C++17 style
/* ... use ... */
std::destroy_at(p);
a.deallocate(p, 3);

std::vector<int, MyAllocator<int>> v;   // container parameterized by allocator
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <memory>

int main() {
    std::allocator<int> alloc;
    int* p = alloc.allocate(3);            // uninitialized memory for 3 ints
    for (int i = 0; i < 3; ++i)
        std::allocator_traits<std::allocator<int>>::construct(alloc, p + i, i + 1);

    for (int i = 0; i < 3; ++i) std::cout << p[i] << ' ';   // 1 2 3
    std::cout << '\n';

    for (int i = 0; i < 3; ++i)
        std::allocator_traits<std::allocator<int>>::destroy(alloc, p + i);
    alloc.deallocate(p, 3);
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `allocate` returns **raw, uninitialized** storage — you must construct objects (placement-new via `allocator_traits`) before use and destroy them before `deallocate`. Forgetting either is UB/leak.
- ⚠️ Always go through `std::allocator_traits<A>` rather than calling allocator members directly — it provides the portable interface (some members were removed in C++17/20).
- ⚠️ Two containers with different allocator types are different types and generally don't interoperate.
- 💡 Most code should **never** write a custom allocator. Reach for one only after profiling shows allocation is the bottleneck (games, HFT, embedded). Consider `std::pmr` (polymorphic memory resources, C++17) for easier pooling.
- 💡 C++17's `<memory_resource>` (`std::pmr::vector`, `monotonic_buffer_resource`) gives runtime-polymorphic allocators without changing the container type.

*(Source: https://en.cppreference.com/w/cpp/memory/allocator)*

**➤ Try it yourself:** Use a `std::pmr::monotonic_buffer_resource` backed by a stack array to allocate a `std::pmr::vector<int>` and observe that it never calls global `new`.

---

<a id="t5-algorithms"></a>
## 5.6 Algorithms (`<algorithm>`): Sorting, Searching, Transforming, Partitioning

`<algorithm>` provides ~100 generic functions operating on iterator ranges, so you express *what* not *how*: `sort`, `find`/`find_if`, `binary_search`/`lower_bound`, `transform`, `copy_if`, `accumulate` (in `<numeric>`), `partition`, `unique`, `count_if`, and more. They're well-tested, optimized, and clearer than hand-rolled loops ("no raw loops").

**Syntax**

```cpp
std::sort(first, last[, cmp]);
auto it = std::find_if(first, last, pred);
std::transform(first, last, out, unaryOp);
auto [lo, hi] = std::equal_range(first, last, value);
last = std::remove_if(first, last, pred);   // erase-remove idiom
```

**Minimal compileable example**

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{5, 3, 8, 1, 9, 2, 7};

    std::sort(v.begin(), v.end());                    // 1 2 3 5 7 8 9
    bool has8 = std::binary_search(v.begin(), v.end(), 8);   // true

    // erase-remove: drop even numbers
    v.erase(std::remove_if(v.begin(), v.end(),
                           [](int x){ return x % 2 == 0; }),
            v.end());

    std::cout << "has8=" << has8 << " remaining: ";
    for (int x : v) std::cout << x << ' ';            // 1 3 5 7 9
    std::cout << '\n';
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `std::remove`/`remove_if` **don't erase** — they shuffle kept elements to the front and return the new logical end. You must follow with `container.erase(...)` (the **erase-remove idiom**).
- ⚠️ `binary_search`, `lower_bound`, and friends require the range to be **sorted** by the same comparator; otherwise results are undefined.
- ⚠️ Comparators must define a **strict weak ordering** (irreflexive, transitive). A buggy comparator (`<=` instead of `<`) causes UB in `sort`.
- 💡 Prefer algorithms + lambdas over manual loops: fewer off-by-one bugs and clearer intent. C++17's parallel overloads (5.12) can speed them up.
- 💡 *(C++20+ — informational only)* C++20 ranges let you write `std::ranges::sort(v)` and compose pipelines with `|`.

*(Source: https://en.cppreference.com/w/cpp/algorithm)*

**➤ Try it yourself:** Given a `vector<std::string>`, use `std::transform` to uppercase each string and `std::partition` to move all strings shorter than 4 chars to the front.

---

<a id="t5-numeric"></a>
## 5.7 Numeric Algorithms (`<numeric>`): `accumulate`, `reduce`, `inner_product`

`<numeric>` holds fold/scan-style algorithms. **`accumulate`** folds a range left-to-right with `+` (or a custom op) — strictly sequential. **`reduce`** (C++17) is like `accumulate` but may run **out of order / in parallel** (requires associativity/commutativity). **`inner_product`** computes the dot product (sum of pairwise products). Also: `partial_sum`, `iota`, and C++17's `transform_reduce`/`exclusive_scan`/`inclusive_scan`.

**Syntax**

```cpp
#include <numeric>
auto s   = std::accumulate(first, last, T{0});           // sequential fold
auto s2  = std::reduce(first, last, T{0});                // C++17, reorderable
auto dot = std::inner_product(f1, l1, f2, T{0});
std::iota(first, last, start);                            // fill 0,1,2,...
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> a{1,2,3,4};
    std::vector<int> b{10,20,30,40};

    int sum = std::accumulate(a.begin(), a.end(), 0);          // 10
    int prod = std::accumulate(a.begin(), a.end(), 1,
                               std::multiplies<>{});           // 24
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);  // 300

    std::vector<int> idx(5);
    std::iota(idx.begin(), idx.end(), 1);                      // 1 2 3 4 5

    std::cout << sum << ' ' << prod << ' ' << dot << '\n';     // 10 24 300
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ The **initial value's type drives the result type**: `std::accumulate(v.begin(), v.end(), 0)` on a `vector<double>` accumulates in `int` and truncates. Pass `0.0`.
- ⚠️ `std::reduce` may reorder operations, so the op must be **associative and commutative** — fine for `+` on ints, *not* fine for floating-point if you need bit-exact reproducibility, or for subtraction.
- ⚠️ `inner_product` needs both ranges to have at least the length of the first; mismatched lengths read out of bounds.
- 💡 Use `std::transform_reduce` (C++17) to fuse a map and a fold (e.g. sum of squares) — and it parallelizes with an execution policy.

*(Source: https://en.cppreference.com/w/cpp/numeric)*

**➤ Try it yourself:** Compute the mean and variance of a `vector<double>` using `std::accumulate` (or `transform_reduce`) without writing an explicit loop.

---

<a id="t5-functional"></a>
## 5.8 Function Objects, Binders (`bind`), `std::invoke`

A **function object** (functor) is any type with `operator()` — including lambdas. The standard provides ready-made functors (`std::plus<>`, `std::less<>`, `std::greater<>`) for passing operations to algorithms. **`std::bind`** creates a callable with some arguments pre-bound (largely superseded by lambdas). **`std::invoke`** (C++17) uniformly calls *any* callable — functions, member functions, member data — handling the awkward member-pointer syntax.

**Syntax**

```cpp
std::sort(b, e, std::greater<>{});                 // descending
auto f = std::bind(g, 10, std::placeholders::_1);  // pre-bind first arg
std::invoke(callable, args...);                     // call anything uniformly
```

**Minimal compileable example**

```cpp
#include <functional>
#include <iostream>

struct Greeter {
    std::string prefix;
    void hello(const std::string& who) const {
        std::cout << prefix << who << '\n';
    }
};

int add(int a, int b) { return a + b; }

int main() {
    std::cout << std::invoke(add, 2, 3) << '\n';        // 5 (free function)

    Greeter g{"Hi, "};
    std::invoke(&Greeter::hello, g, "Ada");             // member function call

    auto add10 = std::bind(add, 10, std::placeholders::_1);
    std::cout << add10(5) << '\n';                       // 15

    auto prefer_lambda = [](int x){ return add(10, x); };  // clearer than bind
    std::cout << prefer_lambda(5) << '\n';              // 15
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ `std::bind` is verbose, error-prone with placeholders, and nests poorly. **Prefer lambdas** in modern C++ — they're clearer and often faster.
- ⚠️ Calling a member function pointer directly needs ugly `(obj.*pmf)(args)` syntax; `std::invoke(pmf, obj, args)` hides it — use it in generic code.
- 💡 Use the transparent operators `std::less<>{}` (note the empty angle brackets) so they deduce argument types and avoid unwanted conversions.
- 💡 `std::invoke` is the foundation for `std::function`, `std::thread`, `std::async`, etc. — they all "invoke" a callable the same way.

*(Source: https://en.cppreference.com/w/cpp/utility/functional/invoke)*

**➤ Try it yourself:** Replace a `std::bind` expression with an equivalent lambda, then call a member function two ways: `(obj.*pmf)(x)` and `std::invoke(pmf, obj, x)`.

---

<a id="t5-concurrency"></a>
## 5.9 Concurrency: `thread`, `mutex`, `lock_guard`, `unique_lock`, `condition_variable`

`<thread>` launches concurrent execution; `<mutex>` protects shared data from data races. Always lock a mutex via an RAII wrapper — **`std::lock_guard`** (simple scoped lock) or **`std::unique_lock`** (movable, deferrable, works with condition variables) — never lock/unlock manually. **`std::condition_variable`** lets threads wait efficiently until another thread signals a state change.

**Syntax**

```cpp
std::thread t(func, args...);  t.join();          // or t.detach()
std::mutex m;
{ std::lock_guard<std::mutex> lk(m); /* critical section */ }
std::unique_lock<std::mutex> ul(m);
cv.wait(ul, []{ return ready; });                 // releases lock while waiting
cv.notify_one();
```

**Minimal compileable example**

```cpp
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;
bool done = false;

void producer() {
    for (int i = 1; i <= 3; ++i) {
        { std::lock_guard<std::mutex> lk(m); q.push(i); }
        cv.notify_one();
    }
    { std::lock_guard<std::mutex> lk(m); done = true; }
    cv.notify_one();
}

void consumer() {
    for (;;) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{ return !q.empty() || done; });   // wait predicate
        while (!q.empty()) { std::cout << "got " << q.front() << '\n'; q.pop(); }
        if (done) break;
    }
}

int main() {
    std::thread p(producer), c(consumer);
    p.join(); c.join();
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A **data race** (two threads touch the same memory, one writing, without synchronization) is undefined behavior — even if it "seems to work". Protect all shared mutable state.
- ⚠️ Every `std::thread` must be `join()`ed or `detach()`ed before destruction, or the program calls `std::terminate`. (RAII wrappers like `std::jthread` solve this — *C++20+, informational only*.)
- ⚠️ **Always wait on a condition variable with a predicate** (`cv.wait(lk, pred)`) to handle spurious wakeups and lost notifications.
- ⚠️ Lock multiple mutexes in a consistent order (or use `std::scoped_lock`, C++17) to avoid **deadlock**.
- 💡 Use `std::lock_guard` for plain scopes; `std::unique_lock` when you need to unlock early, defer, or use a condition variable. Use `std::scoped_lock` (C++17) to lock several mutexes deadlock-free.

*(Source: https://en.cppreference.com/w/cpp/thread)*

**➤ Try it yourself:** Add a second consumer thread and confirm (with a mutex around `std::cout`) that items are consumed exactly once across both.

---

<a id="t5-atomics"></a>
## 5.10 Atomics: `std::atomic`, `memory_order`

`std::atomic<T>` provides operations that are indivisible across threads — no torn reads/writes and no need for a mutex on a single variable. **Memory ordering** controls how surrounding non-atomic operations may be reordered around an atomic op: `seq_cst` (default, simplest, total order), `acquire`/`release` (pairing for producer/consumer handoff), and `relaxed` (atomicity only, no ordering — e.g. counters).

**Syntax**

```cpp
#include <atomic>
std::atomic<int> n{0};
n.fetch_add(1, std::memory_order_relaxed);     // atomic increment
int v = n.load(std::memory_order_acquire);
n.store(5, std::memory_order_release);
bool ok = n.compare_exchange_strong(expected, desired);
```

**Minimal compileable example**

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::atomic<long> counter{0};
    auto work = [&]{ for (int i = 0; i < 100000; ++i)
                         counter.fetch_add(1, std::memory_order_relaxed); };

    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i) ts.emplace_back(work);
    for (auto& t : ts) t.join();

    std::cout << counter.load() << '\n';   // 400000 — no race, no mutex
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Atomicity ≠ ordering. `relaxed` guarantees the operation isn't torn but gives **no** ordering with other memory — fine for independent counters, wrong for "publish data then set a flag" handshakes (use release/acquire there).
- ⚠️ `std::atomic<T>` is only lock-free for small trivially-copyable `T`; large `T` may use an internal lock (`is_lock_free()` tells you).
- ⚠️ Default `seq_cst` is the easiest to reason about but the slowest. Don't reach for `relaxed`/`acquire`/`release` until you've measured *and* understand the happens-before model.
- 💡 Prefer higher-level tools (mutexes, `future`) unless atomics are demonstrably needed; lock-free programming is genuinely hard to get right.

*(Source: https://en.cppreference.com/w/cpp/atomic/atomic)*

**➤ Try it yourself:** Implement a spinlock with `std::atomic_flag` and `test_and_set`/`clear`, then use it to guard a shared counter.

---

<a id="t5-futures"></a>
## 5.11 Futures & Promises: `std::async`, `std::future`, `std::packaged_task`

These tools express "compute a value elsewhere, get it later" without manual thread/lock management. **`std::async`** runs a callable (possibly on another thread) and returns a **`std::future<T>`** whose `.get()` blocks until the result (or rethrows the exception). **`std::promise`** is the producer side you set manually; **`std::packaged_task`** wraps a callable so its result feeds a future — useful for thread pools.

**Syntax**

```cpp
#include <future>
std::future<int> f = std::async(std::launch::async, compute, arg);
int result = f.get();                       // blocks; rethrows exceptions

std::promise<int> p; auto fut = p.get_future();
p.set_value(42);                            // producer sets the value
```

**Minimal compileable example**

```cpp
#include <future>
#include <iostream>

int slow_square(int x) { return x * x; }    // pretend this is expensive

int main() {
    std::future<int> f = std::async(std::launch::async, slow_square, 9);
    // ... do other work concurrently ...
    std::cout << "answer: " << f.get() << '\n';   // 81 (waits if not ready)

    std::promise<int> p;
    std::future<int> pf = p.get_future();
    std::thread([&p]{ p.set_value(7); }).join();
    std::cout << "promised: " << pf.get() << '\n'; // 7
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A `std::future` from `std::async` **blocks in its destructor** until the task finishes if you never call `.get()`/`.wait()`. An unkept `std::async(...)` temporary runs synchronously at the semicolon — surprising. Store the future.
- ⚠️ Without an explicit launch policy, `std::async` *may* run lazily (deferred) on `.get()` instead of concurrently. Pass `std::launch::async` to force a new thread.
- ⚠️ `future::get()` can be called **once**; calling twice is UB. Use `std::shared_future` for multiple consumers.
- ⚠️ Exceptions thrown in the task are stored and **rethrown** at `.get()` — wrap `get()` in try/catch.
- 💡 `std::async` is the simplest way to parallelize independent computations; `packaged_task` is the building block for a custom thread pool.

*(Source: https://en.cppreference.com/w/cpp/thread/async)*

**➤ Try it yourself:** Launch four `std::async` tasks computing partial sums of a large vector, then combine their futures' results.

---

<a id="t5-parallel"></a>
## 5.12 Parallel STL (C++17 execution policies — overview)

C++17 added **execution policies** that let many `<algorithm>`/`<numeric>` functions run in parallel by passing a policy as the first argument: `std::execution::seq` (sequential), `par` (parallel), and `par_unseq` (parallel + vectorized). You opt in with a one-word change, and the library handles the threading.

**Syntax**

```cpp
#include <execution>
std::sort(std::execution::par, v.begin(), v.end());
std::for_each(std::execution::par_unseq, v.begin(), v.end(), f);
auto s = std::reduce(std::execution::par, v.begin(), v.end());
```

**Minimal compileable example**

```cpp
#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v(1'000'000);
    for (std::size_t i = 0; i < v.size(); ++i) v[i] = static_cast<int>(v.size() - i);

    std::sort(std::execution::par, v.begin(), v.end());   // parallel sort
    std::cout << v.front() << " .. " << v.back() << '\n'; // 1 .. 1000000
    return 0;
}
```

```bash
# Some stdlibs need a parallel backend (e.g. Intel TBB) linked:
g++ -std=c++17 -O2 main.cpp -ltbb -o app      # libstdc++ parallel policies
```

**Common pitfalls / Notes**
- ⚠️ The callables you pass to a `par`/`par_unseq` algorithm **must not introduce data races** — they run concurrently. `par_unseq` additionally forbids any synchronization (no locks/atomics) inside the element function.
- ⚠️ Parallel policies are a **permission, not a guarantee** — the implementation may run sequentially (e.g. if no backend is available). With libstdc++ you typically must link Intel **TBB** (`-ltbb`).
- ⚠️ Parallelism has overhead; for small ranges `seq` is faster. Measure before parallelizing.
- 💡 Exceptions escaping a parallel element function call `std::terminate` (they can't propagate cleanly across workers). Handle errors inside the callable.

*(Source: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t)*

**➤ Try it yourself:** Benchmark `std::sort` with `seq` vs `par` on a 10M-element vector and report the speedup on your machine.

---

<a id="t5-type-traits"></a>
## 5.13 Type Traits (`<type_traits>`) & Metaprogramming Intro

Type traits are compile-time queries and transformations on types: predicates (`std::is_integral_v<T>`, `std::is_pointer_v<T>`), relationships (`std::is_same_v<A,B>`, `std::is_base_of_v<B,D>`), and transformations (`std::remove_reference_t<T>`, `std::decay_t<T>`, `std::conditional_t<C,A,B>`). They power generic libraries: you branch and adapt code on type properties entirely at compile time (with `if constexpr`, 4.11).

**Syntax**

```cpp
#include <type_traits>
std::is_integral_v<T>            // bool, compile-time
std::is_same_v<T, int>
std::remove_const_t<T>           // strips const
std::conditional_t<cond, A, B>   // picks a type at compile time
static_assert(std::is_copy_constructible_v<T>, "must be copyable");
```

**Minimal compileable example**

```cpp
#include <iostream>
#include <type_traits>

template<class T>
auto abs_value(T x) {
    static_assert(std::is_arithmetic_v<T>, "abs_value requires a number");
    if constexpr (std::is_unsigned_v<T>) return x;          // already >= 0
    else return x < 0 ? -x : x;
}

int main() {
    std::cout << abs_value(-5) << '\n';        // 5
    std::cout << abs_value(3.5) << '\n';       // 3.5
    std::cout << std::is_same_v<int, signed int> << '\n';   // 1
    // abs_value("x");  // static_assert fires: not arithmetic
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ Use the C++17 `_v` (value) and `_t` (type) shortcuts — `std::is_integral_v<T>` not `std::is_integral<T>::value`, `std::decay_t<T>` not `typename std::decay<T>::type`.
- ⚠️ `static_assert` gives clean, early compile errors with your own message — far better than a deep template error. Use it to document and enforce template requirements.
- ⚠️ Traits operate on **types**, not values; `decltype` bridges expressions to types when needed.
- 💡 Metaprogramming in C++17 is mostly `if constexpr` + traits — much simpler than the old recursive template tricks. Reach for those first.

*(Source: https://en.cppreference.com/w/cpp/header/type_traits)*

**➤ Try it yourself:** Write a `template<class T> void describe()` that uses `static_assert` plus `if constexpr` to print "pointer", "integer", or "other" for `T`.

---

<a id="t5-constexpr"></a>
## 5.14 Compile-Time Computation: `constexpr`, `consteval`, `std::integral_constant`

`constexpr` marks variables/functions usable in **constant expressions** — computed at compile time when inputs are constant, with zero runtime cost (a `constexpr` function may also run at runtime with non-const inputs). C++17 greatly expanded what `constexpr` functions can do (loops, locals, `if`) and added `constexpr` lambdas and `if constexpr`. `std::integral_constant<T,v>` wraps a compile-time value as a type (the basis of `std::true_type`/`false_type`).

**Syntax**

```cpp
constexpr int square(int x) { return x * x; }
constexpr int n = square(8);          // computed at compile time
static_assert(square(8) == 64);
using two = std::integral_constant<int, 2>;   // value-as-type; two::value == 2
```

**Minimal compileable example**

```cpp
#include <array>
#include <iostream>
#include <type_traits>

constexpr long factorial(int n) {      // C++17: loops allowed in constexpr
    long r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

int main() {
    constexpr long f5 = factorial(5);          // computed at compile time
    static_assert(f5 == 120);
    std::array<int, factorial(4) / 12> arr{};  // 2 — constexpr in a type!
    std::cout << f5 << ' ' << arr.size() << '\n';   // 120 2

    std::cout << std::true_type::value << '\n';     // 1 (integral_constant)
    return 0;
}
```

**Common pitfalls / Notes**
- ⚠️ A `constexpr` function called with **non-constant** arguments simply runs at runtime — `constexpr` is a *permission*, not a *guarantee*. Force compile-time eval by assigning to a `constexpr` variable or using `static_assert`.
- ⚠️ Not everything is allowed in a `constexpr` function (no runtime allocation pre-C++20, no `try` in C++17 outside narrow cases). The rules loosened in each standard.
- 💡 **`consteval`** *(C++20+ — informational only)* forces compile-time evaluation (an "immediate function"); there is no `consteval` in C++17 — use `constexpr` + a `constexpr` variable/`static_assert` to guarantee it.
- 💡 Prefer `constexpr` constants over `#define` and over `const` when the value is known at compile time — they're typed and usable in constant expressions (array sizes, template args).

*(Source: https://en.cppreference.com/w/cpp/language/constexpr)*

**➤ Try it yourself:** Write a `constexpr bool is_prime(int)` and use it in a `static_assert` and to size a `std::array`.

---

<a id="t5-debugging"></a>
## 5.15 Debugging & Sanitizers (ASan / TSan / UBSan), Static Analysis

Sanitizers are compiler-instrumented runtime detectors that catch bugs tests often miss. **ASan** (AddressSanitizer) finds use-after-free, buffer overflows, and leaks. **UBSan** finds undefined behavior (signed overflow, bad shifts, null deref). **TSan** (ThreadSanitizer) finds data races. Combine with a debugger (`gdb`/`lldb`) and **static analysis** (`clang-tidy`, `cppcheck`, compiler `-Wall -Wextra`) for defense in depth.

**Syntax / commands**

```bash
g++ -std=c++17 -g -fsanitize=address,undefined prog.cpp -o prog && ./prog
g++ -std=c++17 -g -fsanitize=thread prog.cpp -o prog && ./prog   # TSan (separate)
clang-tidy prog.cpp -- -std=c++17                                 # static analysis
gdb ./prog                                                        # interactive debugger
```

**Minimal compileable example** — a heap-use-after-free ASan catches instantly:

```cpp
#include <iostream>

int main() {
    int* p = new int[3]{1, 2, 3};
    delete[] p;
    std::cout << p[0] << '\n';   // ⚠️ heap-use-after-free
    return 0;
}
```

```bash
g++ -std=c++17 -g -fsanitize=address use_after_free.cpp -o uaf
./uaf      # ASan prints a detailed report: "heap-use-after-free", with stacks
```

**Common pitfalls / Notes**
- ⚠️ ASan and TSan are **incompatible** in the same build — run them in separate builds. ASan+UBSan combine fine.
- ⚠️ Sanitizers add runtime/memory overhead (ASan ~2×); use them in **dev/CI**, not production builds.
- ⚠️ A clean test run without sanitizers proves little about memory/UB correctness — UB can appear to "work" until the compiler/optimizer changes. Run the suite under ASan+UBSan in CI.
- 💡 Compile with `-g` for readable sanitizer/debugger stack traces, and treat warnings as errors (`-Werror`) once the codebase is clean.
- 💡 `-fsanitize=undefined` is cheap and catches a huge class of latent bugs — make it part of your default debug build.

*(Source: https://en.cppreference.com/w/cpp/tools)*

**➤ Try it yourself:** Compile the example above first *without* sanitizers (it may print garbage or "work"), then *with* `-fsanitize=address` and read the report.

---

<a id="t5-build-systems"></a>
## 5.16 Build Systems (CMake) & Package Managers (Conan / vcpkg)

Real projects use a build system to manage compilation, dependencies, and configuration portably. **CMake** is the de facto standard: you describe targets and their requirements, and it generates native builds (Make, Ninja, MSVC). **Package managers** — **vcpkg** (Microsoft) and **Conan** (Python-based) — fetch and build third-party libraries and integrate with CMake, replacing manual dependency hunting.

**Syntax**

```cmake
cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app src/main.cpp)
find_package(fmt CONFIG REQUIRED)           # provided by vcpkg/Conan
target_link_libraries(app PRIVATE fmt::fmt)
target_compile_options(app PRIVATE -Wall -Wextra -pedantic)
```

**Minimal example — build with a package manager (vcpkg)**

```bash
# install a dependency
vcpkg install fmt

# configure CMake to use vcpkg's toolchain, then build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/app
```

```bash
# Conan alternative (conanfile.txt lists requirements like fmt/10.2.1)
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
cmake --build build
```

**Common pitfalls / Notes**
- ⚠️ Prefer **target-based** ("modern") CMake — `target_link_libraries`/`target_include_directories` with `PRIVATE`/`PUBLIC`/`INTERFACE` — over global `include_directories`/`link_libraries`, which leak settings to all targets.
- ⚠️ Set the standard via `CMAKE_CXX_STANDARD` + `CMAKE_CXX_STANDARD_REQUIRED ON` (and `CMAKE_CXX_EXTENSIONS OFF`) rather than hardcoding `-std=c++17` in flags.
- ⚠️ Out-of-source builds (`-B build`) keep the tree clean; never commit the `build/` directory.
- 💡 vcpkg is simplest to start with on Windows/CMake; Conan offers more flexibility (versions, profiles, binary caching). Both expose libraries to `find_package`.
- 💡 Pin dependency versions (vcpkg manifest `vcpkg.json`, Conan `conanfile`) for reproducible builds across machines and CI.

*(Source: https://cmake.org/cmake/help/latest/)*

**➤ Try it yourself:** Create a CMake project that depends on `fmt` via vcpkg or Conan, and print "Hello, {}!" with `fmt::format`.

---

### ✅ Tier 5 Checkpoint

You can now: select containers by complexity and access pattern; reason about iterator categories; use `<algorithm>`/`<numeric>` instead of raw loops; apply functors/`bind`/`invoke`; write race-free multithreaded code with mutexes/locks/condition variables; use atomics and memory orders; run async work with futures; parallelize with execution policies; metaprogram with traits and `constexpr`; and operate the professional toolchain (sanitizers, CMake, package managers).

**Self-test:** Build a small word-frequency tool: read a file with `std::filesystem`/streams, tokenize into a `std::unordered_map<std::string,int>`, sort the top 10 by count with `std::sort` over a `vector` of pairs, parallelize the counting of multiple files with `std::async`, and run the whole thing under `-fsanitize=address,undefined`.

---

<a id="appendix-cheatsheet"></a>
---

# 📎 Appendix A — Cheat Sheet (dense, by tier)

**Tier 1 — Ground Zero**
```cpp
g++ -std=c++17 -Wall -Wextra -pedantic f.cpp -o f   // compile
int main(){ std::cout << "hi\n"; }                   // entry point
int n{42}; double d{3.14}; bool b{true};             // brace-init (no narrowing)
auto x = expr;  decltype(expr) y = expr;             // type deduction
if(c){} else{}  switch(n){case 1: ...; break;}       // control flow
for(int i=0;i<n;++i){}  while(c){}  do{}while(c);
ret f(params);  /* decl */   ret f(params){...}      // function decl/def
std::cin >> x;  std::getline(std::cin, s);           // input
#include <h>   using std::cout;   namespace n{}      // headers/namespaces
```

**Tier 2 — Foundations**
```cpp
int a[]{1,2,3};  std::size(a);                       // array + safe size
std::string s; std::string_view sv{s};               // owning vs view (no copy)
int* p=&x; *p; nullptr;  int& r=x;                    // pointer / reference
auto u=std::make_unique<T>(...);                      // prefer over new/delete
struct S{int x;};  enum class E{A,B};                 // aggregates / scoped enum
#pragma once                                          // header guard
{ RAII g; }   // ctor acquires, dtor releases (auto)  // RAII
```

**Tier 3 — OOP**
```cpp
class C { public: C(); ~C(); private: int v_{}; };    // encapsulation
// Rule of 5: ~C, C(const C&), op=(const C&), C(C&&)noexcept, op=(C&&)noexcept
// Rule of 0: hold vector/unique_ptr -> declare none
struct D : public Base { void f() override; };         // is-a + override
virtual ~Base()=default;  virtual T g()=0;             // polymorphic base / pure
static_cast<T>(x); dynamic_cast<T*>(p); const_cast; reinterpret_cast;
// SOLID; prefer composition (has-a) over inheritance
```

**Tier 4 — Modern C++17**
```cpp
auto y=std::move(x); std::forward<T>(a);              // move / perfect forward
make_unique/make_shared; weak_ptr breaks cycles       // ownership
template<class T> ...;  template<class...Ts>(args+...) // templates / fold
if constexpr(std::is_integral_v<T>){}                  // compile-time branch
[cap](auto x){...};                                    // (generic) lambda
auto [a,b]=pair;  optional<T>; variant<...>; any;      // C++17 vocab types
namespace fs=std::filesystem;                          // filesystem
if(auto it=m.find(k); it!=m.end()){}                   // init-statement
inline constexpr int k=1;  [[nodiscard]] [[fallthrough]]
try{throw E{};}catch(const std::exception& e){}  noexcept
```

**Tier 5 — STL & Toolkit**
```cpp
vector(default) array deque list; map/set; unordered_map/set
std::sort(b,e); find_if; transform; v.erase(remove_if(...),v.end());
std::accumulate(b,e,0); reduce; inner_product; iota;
std::invoke(f,args...);  std::less<>{}                 // functors
std::thread t(f); t.join(); lock_guard/unique_lock; cv.wait(lk,pred);
std::atomic<int> n; n.fetch_add(1,relaxed);
auto fut=std::async(std::launch::async,f,arg); fut.get();
std::sort(std::execution::par,b,e);                    // parallel STL
std::is_same_v<A,B>; decay_t; static_assert(cond,"msg");
constexpr T f(...);                                    // compile-time
g++ -fsanitize=address,undefined -g ...                // sanitizers
cmake -S . -B build && cmake --build build             // build
```

---

<a id="appendix-study-plan"></a>
---

# 📎 Appendix B — 12-Week Study Plan

| Week | Focus | Tier / Sections | Milestone project |
|---|---|---|---|
| 1 | Toolchain, compilation, Hello World, variables, I/O | T1 §1.1–1.5, 1.9 | Compile from CLI + CMake; temperature converter |
| 2 | Operators, control flow, functions, headers, const/namespaces | T1 §1.6–1.8, 1.10–1.11 | Number-guessing game with helper functions |
| 3 | Arrays, strings/`string_view`, pointers vs references | T2 §2.1–2.4 | Word-count over a fixed C-string |
| 4 | `new`/`delete`, struct/class, enums, function objects, RAII, semantics | T2 §2.5–2.11 | RAII buffer class; replace with `vector` |
| 5 | Classes, ctors/dtors, Rule of 5/0, `this`/`static`/`const` | T3 §3.1–3.4 | A safe `Matrix` class (Rule of 0) |
| 6 | Operator overloading, inheritance, polymorphism, abstract classes | T3 §3.5–3.8 | Shape hierarchy with virtual `area()` |
| 7 | Multiple/virtual inheritance, casts, SOLID, composition | T3 §3.9–3.12 | Refactor an inheritance design to composition |
| 8 | Move semantics, smart pointers, templates | T4 §4.1–4.3 | Generic move-only `unique_resource` |
| 9 | Variadics/folds, SFINAE/`if constexpr`, lambdas | T4 §4.4–4.7 | A small type-safe `print(...)` and `apply` |
| 10 | Structured bindings, optional/variant/any, filesystem, attributes, exceptions | T4 §4.8–4.14 | Config loader returning `optional`/`variant` |
| 11 | Containers, iterators, algorithms, numeric, functors | T5 §5.1–5.8 | Top-N word frequency with `<algorithm>` |
| 12 | Concurrency, atomics, futures, parallel STL, traits/`constexpr`, tooling | T5 §5.9–5.16 | Multithreaded file processor + ASan/CMake/vcpkg |

> ⏱️ Plan assumes ~8–10 focused hours/week. Slower pace? Spread Tiers 4–5 over extra weeks — they're the densest.

---

<a id="appendix-interview"></a>
---

# 📎 Appendix C — 100 Essential C++17 Interview Questions

### Tier 1 — Fundamentals (1–18)
1. What are the three phases of building a C++ program?
2. What's the difference between a declaration and a definition?
3. Why does `main` return `int`, and what does the return value mean?
4. What's the difference between `'\n'` and `std::endl`?
5. What does brace `{}` initialization protect against that `=` doesn't?
6. Why are the sizes of `int`/`long` not fixed, and how do you get a fixed-width integer?
7. What happens if you read an uninitialized local variable?
8. How does `auto` deduce types, and what does it drop (`const`, references)?
9. What's the difference between `auto` and `decltype`?
10. Why is `7 / 2 == 3`? How do you get `3.5`?
11. What is the difference between `i++` and `++i`?
12. What is `switch` fall-through, and how do you mark it intentional?
13. How does function overloading resolve? Can overloads differ only by return type?
14. When should you pass by value vs `const&`?
15. Where do default arguments belong — declaration or definition?
16. What's the difference between `std::cin >> x` and `std::getline`?
17. Why is `using namespace std;` discouraged in headers?
18. What's the difference between `<header>` and `"header"` includes?

### Tier 2 — Memory & Types (19–38)
19. Why don't built-in arrays know their own size? What is array decay?
20. What is a C-string, and how is its length determined?
21. `std::string` vs `std::string_view` — when do you use each?
22. What is a dangling `string_view`, and how do you avoid it?
23. What's the difference between a pointer and a reference?
24. Why prefer `nullptr` over `NULL` or `0`?
25. What is pointer arithmetic, and when is it valid?
26. What is a `void*` and what must you do before using it?
27. What problems arise from raw `new`/`delete`? Name three.
28. What's the difference between `delete` and `delete[]`?
29. What is the only language difference between `struct` and `class`?
30. Unscoped `enum` vs `enum class` — what does scoping/strong-typing buy you?
31. How do you get the integer value of an `enum class`?
32. When does a lambda convert to a function pointer?
33. What does `std::function` cost compared to a raw function pointer?
34. What is a header guard, and how does `#pragma once` differ from `#ifndef`?
35. What is RAII, and why does it remove the need for `finally`?
36. What's the difference between stack and heap allocation?
37. Value semantics vs reference semantics — give an example of each.
38. What is object aliasing, and why is it a hazard?

### Tier 3 — OOP (39–60)
39. What are the three access specifiers and what do they control?
40. Why initialize members in the initializer list instead of the body?
41. In what order are class members initialized?
42. State the Rule of 5. State the Rule of 0. Which should you prefer and why?
43. Why does a defaulted copy on a class with a raw owning pointer cause a double free?
44. Why should move constructors be `noexcept`?
45. What state must a moved-from object be left in?
46. What is `this`, and why return `*this`?
47. What's a `static` member, and how did C++17 change its definition?
48. What does a `const` member function promise? What is `mutable` for?
49. Which operators must be members? Which must be non-members?
50. Why implement `+` in terms of `+=` and `!=` in terms of `==`?
51. What does `public` inheritance model (the substitutability rule)?
52. What does `override` protect against? What does `final` do?
53. What's the difference between overriding and name hiding?
54. How do virtual functions work (vtable/vptr)? What's the cost?
55. **Why must a polymorphic base class have a virtual destructor?**
56. What is object slicing, and how do you avoid it?
57. Why shouldn't you call virtual functions from a constructor?
58. What is a pure virtual function, and what makes a class abstract?
59. What is the diamond problem, and how does virtual inheritance solve it?
60. Compare the four C++ casts and give a use case for each.

### Tier 4 — Modern C++17 (61–82)
61. What's the difference between an lvalue and an rvalue?
62. What does `std::move` actually do?
63. What is a forwarding (universal) reference, and when is `T&&` one?
64. What is perfect forwarding, and why use `std::forward`?
65. Why is `return std::move(local);` usually a pessimization?
66. `unique_ptr` vs `shared_ptr` vs `weak_ptr` — when each?
67. Why prefer `make_shared`/`make_unique` over `new`?
68. How does `weak_ptr` break a reference cycle?
69. Why must template definitions usually live in headers?
70. What is template specialization (full vs partial)?
71. What is a variadic template and a parameter pack?
72. What is a fold expression? Why does an empty-pack unary fold sometimes fail?
73. What is SFINAE? How does `std::enable_if` use it?
74. When would you choose `if constexpr` over `std::enable_if`?
75. Explain lambda capture modes; what are the dangers of `[&]` and `[=]` with `this`?
76. What is a generic lambda?
77. What are structured bindings, and what can they bind to?
78. `std::optional` vs returning a pointer/sentinel — why prefer `optional`?
79. `std::variant` vs inheritance — when is `variant` the better model?
80. What does an `if (init; cond)` statement buy you?
81. What problem do inline variables (C++17) solve?
82. What does `[[nodiscard]]` do? Give a good use. When does `noexcept` matter?

### Tier 5 — STL & Systems (83–100)
83. `vector` vs `list` — why is `vector` usually faster even for many insertions?
84. When does a `vector` invalidate iterators/pointers/references?
85. `map` vs `unordered_map` — complexity and when to choose each?
86. What's the hazard of `map::operator[]` when reading? What are the alternatives?
87. Name the iterator categories from least to most capable.
88. Why can't you `std::sort` a `std::list`?
89. What is the erase-remove idiom and why is it needed?
90. What ordering must a comparator satisfy for `std::sort`?
91. `std::accumulate` vs `std::reduce` — what's the key difference and constraint?
92. Why does the initial value's type matter in `accumulate`?
93. Why prefer lambdas over `std::bind`? What does `std::invoke` solve?
94. What is a data race, and why is it UB?
95. `lock_guard` vs `unique_lock` vs `scoped_lock` — when each?
96. Why must you wait on a condition variable with a predicate?
97. What does memory ordering control? Contrast `seq_cst` vs `relaxed`.
98. What's the gotcha with a `std::future` returned from `std::async` (destructor/launch policy)?
99. What guarantees do C++17 execution policies give (and not give)?
100. Which sanitizer catches use-after-free, which catches data races, and why run UBSan in CI?

> 💡 For each, practice a 60–90 second spoken answer plus a 5-line code sketch. The starred (⭐ #55) ones are near-universal in interviews.

---

<a id="appendix-reading"></a>
---

# 📎 Appendix D — Further Reading

**Foundational / language**
- **Bjarne Stroustrup — *A Tour of C++* (2nd ed., C++17).** The fastest authoritative overview from the language's creator. Best companion to this document.
- **Bjarne Stroustrup — *Programming: Principles and Practice Using C++* (2nd ed.).** Ground-up for true beginners (maps to Tiers 1–3).
- **Bjarne Stroustrup — *The C++ Programming Language* (4th ed.).** The comprehensive reference.

**Modern idioms & best practices**
- **Scott Meyers — *Effective Modern C++* (C++11/14).** 42 items on smart pointers, move semantics, lambdas, concurrency. Essential for Tier 4. (Pair with cppreference for C++17 deltas.)
- **Scott Meyers — *Effective C++* (3rd ed.) & *Effective STL*.** Timeless design and STL-usage guidance (Tiers 3 and 5).
- **Herb Sutter — *Exceptional C++* / *More Exceptional C++*.** Exception safety, RAII, and tricky idioms via "GotW" puzzles.

**STL & generic programming**
- **Nicolai Josuttis — *The C++ Standard Library* (2nd ed.).** The definitive STL reference: containers, algorithms, iterators, allocators (core of Tier 5).
- **Nicolai Josuttis — *C++17 — The Complete Guide*.** Deep, focused coverage of every C++17 feature (structured bindings, `optional`/`variant`/`any`, `if constexpr`, fold expressions, parallel STL).
- **Peter Gottschling — *Discovering Modern C++* (2nd ed.).** Modern C++ aimed at scientists/engineers; strong on templates, generic programming, and `constexpr`.

**Reference & standards (treat as ground truth)**
- **cppreference.com** — the working programmer's reference (every section here cites it).
- **eel.is/c++draft** — the ISO C++ standard draft, for precise wording.
- **isocpp.org** + **C++ Core Guidelines** (Stroustrup & Sutter) — official guidance and rationale.
- **learncpp.com** — free, beginner-friendly tutorials mirroring Tiers 1–3.

---

## 🎉 Document Complete

All five tiers (1–5) plus the Cheat Sheet, 12-week study plan, 100 interview questions, and Further Reading are now written. Every concept follows the required format (explanation → syntax → compileable `-std=c++17` example → ⚠️/💡 pitfalls → source), C++20+ features are flagged informational-only, and all examples target `g++ -std=c++17 -Wall -Wextra -pedantic`.

**Suggested next steps:** (1) verify the table-of-contents anchors against the section IDs; (2) optionally compile each example into a `tests/` folder to guarantee they build; (3) tell me any tier you'd like expanded with more examples, diagrams, or benchmarks.

=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
=======================================================================================================
