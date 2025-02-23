# PyInCpp

_A C++ type library that is as easy to use as Python built-in types._

[中文](./readme_zh.md)

## 1. Attribute

- Name: PyInCpp (means **Py**thon **in** **C++**)
- Language: C++, requires C++20
- Goal: Provide a C++ type library that is as easy to use as Python built-in types
- Module: List, Set, Dict, Int, Str, Tuple, Complex, Deque, Fraction
- Style: Most follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), some my own styles are based on considerations of source code size and simplicity
- Document: Use [Doxygen](https://www.doxygen.nl) to generate documents

## 2. Feature

- Simple: Stay simple, stay young. While ensuring friendly and robust, try to be concise and easy to maintain and read
- Friendly: With my careful design, it can be used as conveniently as Python's built-in types. Very Pythonic
- Robust: There are corresponding checks for the insert, remove, modify, and access of containers
- Lightweight: Designed as a single header file, the source code does not rely on any third-party libraries, making it very easy to use and expand
- Efficient: The [benchmark results](./benches) show that the performance of the parts with the same function as the standard library is almost the same, and the big integer module of this project is very fast
- Secure: Tested and checked using [Catch2](https://github.com/catchorg/Catch2) and [Dr. Memory](https://drmemory.org) to ensure no security issues

## 3. Usage

Provided as the form of single header file, header-only, very convenient to use:

- PyInCpp has already in the official [XMake](https://xmake.io) repository, you only need to add it in the xmake.lua: `add_requires("pyincpp")` and then `#include <pyincpp.hpp>`.
- Or, just copy the source files into your project and then `#include "pyincpp.hpp"`.

There are a total of 9 classes, refer to commonly used classes in Python:

| Type in PyInCpp | Type in Python       |
| --------------- | -------------------- |
| `List<T>`       | `list`               |
| `Set<T>`        | `set`                |
| `Dict<K, V>`    | `dict`               |
| `Int`           | `int`                |
| `Str`           | `str`                |
| `Tuple<Ts...>`  | `tuple`              |
| `Complex`       | `complex`            |
| `Deque<T>`      | `collections.deque`  |
| `Fraction`      | `fractions.Fraction` |

Some simple examples:

```cpp
using namespace pyincpp;

// List support negative index
List<int>{1, 2, 3, 4, 5}[-1]; // 5
// List traversal
List<int>{1, 2, 3, 4, 5}.map([](int& x) { x *= 2; }); // [2, 4, 6, 8, 10]
// List uniquify
List<int>{1, 2, 3, 1, 2, 3, 1, 2, 3}.uniquify(); // [1, 2, 3]
// List sort, stable sort, default from small to large, customizable comparator
List<int>{1, 2, 3, 4, 5}.sort([](const int& e1, const int& e2) { return e1 > e2; }); // [5, 4, 3, 2, 1]

// test whether a Set is proper subset of another Set
Set<int>{5, 1} < Set<int>{1, 2, 3, 4, 5}; // true
// intersection of Sets, support intersection, union, difference, and symmetric difference
Set<int>{1, 2, 3, 4, 5} & Set<int>{1, 3, 5, 7, 9}; // {1, 3, 5}

// Dict access
Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}["one"]; // 1
// Dict get values
Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}.values(); // {1, 2, 3}

// Int basic operation, support +, -, *, /, % and compare
Int("18446744073709551617") + Int("18446744073709551617"); // 36893488147419103234
// Int increment, after my optimization, much faster than `+= 1`
++Int("99999999999999"); // 100000000000000
// Int modular power, very fast
Int::pow("1024", "1024", "100"); // 76
// Int factorial
Int("5").factorial().factorial(); // 668950291344912705758811805409037258675274633313802981029567135...
// get random Int of specified number of digits
Int::random(1024); // 237957592143483875146995224963278325109395733362902250996014213111309067269964...
// calculate the next prime that greater than this
Int("7").next_prime(); // 11
// calculate the tetration
Int::hyperoperation("4", "3", "3"); // 7625597484987

// convert Str to floating-point number, support inf and nan
Str(".1e-2").to_decimal(); // 0.1e-2
// convert Str to integer, support base 2-36
Str("-ffffffffffffffff").to_integer(16); // -18446744073709551615
// Str repeat
Str("hello! ") * 2; // "hello! hello! "
// Str replace
Str("hahaha").replace("a", "ooow~ "); // "hooow~ hooow~ hooow~ "
// Str slice
Str("12345").slice(0, 5, 2); // "135"
// Str split
Str("one, two, three").split(", "); // ["one", "two", "three"]
// Str join
Str(".").join({"192", "168", "0", "1"}); // "192.168.0.1"
// Str format
Str("I'm {}, {} years old.").format("Alice", 18); // "I'm Alice, 18 years old."

// Tuple index, return type different, so template function is used
Tuple<int, double, char>(1, 2.5, 'A').get<2>(); // 'A'
// take the remaining part of the Tuple, the underlying layer is pointer conversion, which is very fast
Tuple<int, double, char>(1, 2.5, 'A').rest(); // (2.5, 'A')

// Complex addition
Complex(1, 2) + Complex(1, 3); // (2+5j)
// Complex power
Complex::pow(Complex(1, 2), Complex(-1, 2)); // (0.04281551979798478+0.023517649351954585j)

// Deque element reference
Deque<int>{1, 2, 3, 4, 5}.front(); // 1
// Deque rotate to right (or left), very vivid!
Deque<int>{1, 2, 3, 4, 5} >>= 1; // <5, 1, 2, 3, 4>

// Fraction addition
Fraction(1, 2) + Fraction(1, 3); // 5/6
// Fraction modulo
Fraction(1, 2) % Fraction(1, 3); // 1/6
```

## 4. Advantage

The advantage of PyInCpp is that it combines the high performance of C++ with the ease of use of Python, and can also be easily combined with other libraries, for example:

```cpp
// 1. All types can be printed and easily combined:
Dict<Str, List<Int>> dict = {{"first", {"123", "456"}}, {"second", {"789"}}, {"third", {"12345678987654321", "5"}}};
std::cout << dict; // {"first": [123, 456], "second": [789], "third": [12345678987654321, 5]}
dict.keys(); // {"first", "second", "third"}
dict["third"][-1].factorial(); // 120

// 2. All container types are iterable:
for (const auto& [k, v] : Dict<int, int>{{1, 1}, {2, 4}, {3, 9}})
{
    assert(k * k == v);
}

// 3. All immutable types are hashable:
std::unordered_set<Int> set1 = {"1", "2", "3", "18446744073709551617"};
std::unordered_set<Str> set2 = {"hello", "pyincpp"};
std::unordered_set<Fraction> set3 = {{1, 2}, {3, 4}};

// 4. Combining pyincpp::Fraction with Eigen library to display accurate matrix.
using Matrix = Eigen::Matrix<pyincpp::Fraction, 2, 2>; // compiling with boost::rational will fail

Matrix A = Matrix{{1, 2}, {3, 4}};
Matrix B = Matrix::Zero();
Matrix C = Matrix::Ones();
Matrix D = Matrix::Identity();

std::cout << ((A + B) * (C + D)).inverse() << std::endl;
/*
-11/6     5/6
  5/3    -2/3
*/

// 5. boost::rational vs pyincpp::Fraction
boost::rational<int> r1(1, 2), r2(1, 3), r3(1, 4), r4(1, 5);
pyincpp::Fraction f1(1, 2), f2(1, 3), f3(1, 4), f4(1, 5);

std::cout << ((r1 + r2) * r3 / r4) << std::endl; // 25/24
// std::cout << ((r1 + r2) * r3 % r4) << std::endl; // boost::rational does not support operator%
std::cout << ((f1 + f2) * f3 / f4) << std::endl; // 25/24
std::cout << ((f1 + f2) * f3 % f4) << std::endl; // 1/120

// 6. std::tuple vs boost::tuple vs pyincpp::Tuple
auto t1 = std::make_tuple(1, 1.5, 'A', "hello", std::tuple<std::tuple<>, std::tuple<>>({}, {}));
auto t2 = boost::make_tuple(1, 1.5, 'A', "hello", boost::tuple<boost::tuple<>, boost::tuple<>>({}, {}));
auto t3 = pyincpp::make_tuple(1, 1.5, 'A', "hello", pyincpp::Tuple<pyincpp::Tuple<>, pyincpp::Tuple<>>({}, {}));

// std::cout << t1 << std::endl; // std::tuple does not support operator<<
std::cout << t2 << std::endl; // (1 1.5 A hello (() ()))
std::cout << t3 << std::endl; // (1, 1.5, A, hello, ((), ()))
```

If you want to use a similar library in Rust, please see: [PyInRs](https://github.com/chen-qingyu/pyinrs).

## 5. History

It was originally developed in C to learn data structures. Then in 2021 I tried to implement some handy container classes in C++, just for fun. After several refactoring, the development goals were determined. The original version was migrated to [HelloDS](https://github.com/chen-qingyu/hellods) with the goal of implementing a complete, generic base container class without using any standard library containers. And the goal of this project is to achieve basic types similar to Python's, and to be as convenient and efficient as possible. I started developing this project on January 1, 2023.

A word about inline: for the sake of source brevity, I finally decided to adopt the inline style. There is usually no problem unless there is a high requirement for program size. At first I wrote the declaration and the definition separately, but this is a template, so I can't split it into two files, so I'm going to write it in one file, and I'm going to put inline in front of some of the function definitions, but it ended up being a verbose bunch of "template typename inline". After reading the Java source code, I decided to write it all in the class, as the default inline. Inline is just a request to the compiler, not a requirement, and functions that can not be inline (such as recursive functions) are not executed by the compiler.

Every line of code has been carefully optimized by me. [Comparing](./benches/pyincpp_int_vs_other_int.cpp) pyincpp::Int to some big integer projects on GitHub with over 100 stars, the conclusion is that pyincpp::Int is faster, more feature-rich, and has simpler and cleaner code.

For this project, the simplicity and readability of the source code take precedence over performance optimizations that do not involve time complexity. Therefore, I usually do not perform special optimizations for functions that can be completed using a unified algorithm. For modern people, the utilization rate of computing resources is actually very low, and the marginal diminishing effect of performance improvement is significant. In comparison, human attention is more important. The source code of this project can also be considered as part of its "functionality".

In this project, I used the One-hot code idea in FPGA combined with the finite state machine. I also used template meta-programming to recursively implement any variable template parameters at compile time. It sounds great, but it doesn't make money, not many people really use, just belong to self-entertainment now, but creation is happiness, creation is meaning.
