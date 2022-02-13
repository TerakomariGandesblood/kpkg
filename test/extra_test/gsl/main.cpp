#include <cstdint>

#include <gsl/assert>

int main() {
  std::int32_t a = 10;
  Ensures(a >= 10);
}
