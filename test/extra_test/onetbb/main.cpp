#include <cstdint>

#include <oneapi/tbb.h>

#include "error.h"

std::uint64_t fibonacci(std::uint32_t n) {
  if (n < 2) {
    return n;
  }

  return fibonacci(n - 1) + fibonacci(n - 2);
}

std::uint64_t parallel_fibonacci(std::uint32_t n) {
  if (n < 10) {
    return fibonacci(n);
  }

  std::uint64_t x, y;
  tbb::task_group g;
  g.run([&] { x = fibonacci(n - 1); });
  g.run([&] { y = fibonacci(n - 2); });
  g.wait();

  return x + y;
}

int main() { EXPECT(fibonacci(40) == 102334155); }
