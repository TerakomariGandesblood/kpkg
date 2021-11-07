#include <cstdint>

#include <gsl/gsl-lite.hpp>

int main() {
  std::int32_t a = 10;
  Ensures(a >= 10);
}
