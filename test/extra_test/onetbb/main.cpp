#include <algorithm>
#include <cstdint>
#include <execution>
#include <random>
#include <vector>

#include <oneapi/tbb.h>

#include "error.h"

int main() {
  std::vector<std::int32_t> v;
  constexpr std::int32_t size = 100000;
  v.reserve(size);

  std::random_device rd;
  std::default_random_engine gen(rd());
  for (std::int32_t i = 0; i < size; ++i) {
    v.push_back(static_cast<std::int32_t>(gen()));
  }

  tbb::parallel_sort(v);
  EXPECT(std::is_sorted(std::execution::par, std::begin(v), std::end(v)));
}
