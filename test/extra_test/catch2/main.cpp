#include <cstdint>
#include <vector>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

std::uint64_t factorial(std::uint64_t number) {
  return number > 1 ? factorial(number - 1) * number : 1;
}

// https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
// https://github.com/catchorg/Catch2/blob/devel/docs/benchmarks.md
TEST_CASE("factorial") {
  // 失败则终止 test case
  REQUIRE(factorial(0) == 1);
  // 失败也继续执行
  CHECK(factorial(0) == 1);

  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3628800);

  BENCHMARK("factorial 10") { return factorial(10); };

  BENCHMARK_ADVANCED("factorial 10 advanced")
  (Catch::Benchmark::Chronometer meter) {
    meter.measure([] { return factorial(10); });
  };
}

TEST_CASE("float") {
  REQUIRE([] { return 0.5 + 0.8; }() == Catch::Approx(1.3));
}

SCENARIO("vectors can be sized and resized") {
  GIVEN("A vector with some items") {
    std::vector<std::int32_t> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    WHEN("the size is increased") {
      v.resize(10);

      THEN("the size and capacity change") {
        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
      }
    }
    WHEN("the size is reduced") {
      v.resize(0);

      THEN("the size changes but not capacity") {
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);
      }
    }
    WHEN("more capacity is reserved") {
      v.reserve(10);

      THEN("the capacity changes but not the size") {
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);
      }
    }
    WHEN("less capacity is reserved") {
      v.reserve(0);

      THEN("neither size nor capacity are changed") {
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
      }
    }
  }
}
