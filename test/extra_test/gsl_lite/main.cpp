#include <cstdint>

#ifndef NDEBUG
#define gsl_CONFIG_CONTRACT_VIOLATION_THROWS
#endif
#include <gsl-lite/gsl-lite.hpp>

int main() {
  std::int32_t a = 10;
  Ensures(a >= 10);
}
