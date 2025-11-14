/**
 * @file tuple-support-test.cc
 * @brief Tests and benchmarks for the aux::tuple_like support library.
 *
 * This test suite validates the stream insertion operator (operator<<)
 * for standard C++ tuple-like structures such as std::tuple, std::pair,
 * and std::array, ensuring they conform to the expected output format
 * (e1, e2, ...).
 *
 * @author Nakashima, Terumi
 * @date 2025-11-17
 * @version 1.0
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <aux/tuple-support.hpp>

#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

TEST_CASE("Stream insertion for standard tuple_like structures", "[tuple_support]") {
    using aux::operator<<;
    auto out = std::stringstream{};

    SECTION("std::pair") {
        auto p = std::pair{1, "hello"};
        out << p;
        REQUIRE(out.str() == "(1, hello)");
    }

    SECTION("Empty tuple") {
        auto empty = std::tuple<>{};
        out << empty;
        REQUIRE(out.str() == "()");
    }

    SECTION("Single element tuple") {
        auto single = std::tuple{10};
        out << single;
        REQUIRE(out.str() == "(10)");
    }

    SECTION("Frequent tuple usage") {
        out << std::fixed << std::setprecision(2);
        auto t = std::tuple{3.14, "c++", 42};
        out << t;
        REQUIRE(out.str() == "(3.14, c++, 42)");
    }

    SECTION("Nested tuples", "[tuple_support]") {
        auto nested = std::tuple{1, std::pair{2.5, 'a'}, std::tuple{true}};
        out << nested;
        REQUIRE(out.str() == "(1, (2.5, a), (1))");
    }

    SECTION("std::array (fixed size container)") {
        std::array<int, 4> arr = {1, 2, 3, 4};
        out << arr;
        REQUIRE(out.str() == "(1, 2, 3, 4)");
    }
}

TEST_CASE("Benchmark large tuple", "[tuple_support][benchmark]") {
    using aux::operator<<;
    auto out = std::stringstream{};

    constexpr auto large_tuple = std::tuple{
         1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
        11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
        21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
        31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,  49,  50,
        51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
        61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
        71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
        81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
        91,  92,  93,  94,  95,  96,  97,  98,  99, 100,
    };

    /**
     * @note Benchmarks the pure output performance of a 100-element tuple.
     *
     * We use std::stringstream with a clear operation (out.str("")) instead
     * of std::spanstream (C++23) to remain C++20 compliant.
     * This approach also reflects realistic application scenarios and prevents
     * unnecessary reallocations within the benchmark loop (assuming a typical
     * std::stringstream implementation that reuses its internal buffer).
     */
    BENCHMARK("Output 100-element tuple") {
        out.str("");
        out << large_tuple;
    };
}
