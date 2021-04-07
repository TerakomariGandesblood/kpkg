#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include <tbb/parallel_sort.h>

int main() {
  constexpr std::size_t size = 1'0000'0000;

  std::vector<std::uint64_t> v;
  v.reserve(size);

  std::default_random_engine e{std::random_device{}()};
  for (std::size_t i{0}; i < size; ++i) {
    v.push_back(e());
  }

  const auto start{std::chrono::system_clock::now()};
  tbb::parallel_sort(v);
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now() - start)
                   .count()
            << " ms\n";

  if (!std::is_sorted(std::begin(v), std::end(v))) {
    std::cerr << "error\n";
    return EXIT_FAILURE;
  }
}
