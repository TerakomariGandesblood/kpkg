#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

void dense_range(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<std::int64_t> v(
        static_cast<std::vector<std::int64_t>::size_type>(state.range(0)),
        state.range(0));
    benchmark::DoNotOptimize(v.data());
    benchmark::ClobberMemory();
  }
}

BENCHMARK(dense_range)->Range(8, 512);
BENCHMARK(dense_range)->DenseRange(0, 256, 128);

void sort_vector(benchmark::State& state) {
  std::default_random_engine e{std::random_device{}()};
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<std::uint64_t> v;
    v.reserve(
        static_cast<std::vector<std::int64_t>::size_type>(state.range(0)));

    for (std::int32_t i{0}; i < state.range(0); ++i) {
      v.push_back(e());
    }

    state.ResumeTiming();
    std::sort(std::begin(v), std::end(v));
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(sort_vector)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 12)
    ->Complexity();
