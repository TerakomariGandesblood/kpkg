#include <cstddef>

#include <hwy/aligned_allocator.h>
#include <hwy/highway.h>
#include <klib/log.h>

namespace hn = hwy::HWY_NAMESPACE;

using T = double;

void MulAddLoop(const T *HWY_RESTRICT mul_array,
                const T *HWY_RESTRICT add_array, const std::size_t size,
                T *HWY_RESTRICT x_array) {
  const hn::ScalableTag<T> d;
  for (std::size_t i = 0; i < size; i += hn::Lanes(d)) {
    const auto mul = hn::Load(d, mul_array + i);
    const auto add = hn::Load(d, add_array + i);
    auto x = hn::Load(d, x_array + i);
    x = hn::MulAdd(mul, x, add);
    hn::Store(x, d, x_array + i);
  }
}

int main() {
  const std::size_t count = 1024;
  auto x_array = hwy::AllocateAligned<T>(count);
  auto mul_array = hwy::AllocateAligned<T>(count);
  auto add_array = hwy::AllocateAligned<T>(count);

  for (std::size_t i = 0; i < count; ++i) {
    x_array[i] = i;
    mul_array[i] = i;
    add_array[i] = i;
  }

  MulAddLoop(mul_array.get(), add_array.get(), count, x_array.get());

  for (std::size_t i = 0; i < count; ++i) {
    if (x_array[i] != i * i + i) {
      klib::error("Wrong result");
    }
  }
}
