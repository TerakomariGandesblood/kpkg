#include <cstdint>
#include <string>
#include <vector>

#ifdef NDEBUG
#define DBG_MACRO_DISABLE
#endif

#include <dbg.h>

std::int32_t factorial(std::int32_t n) {
  if (dbg(n <= 1)) {
    return dbg(1);
  } else {
    return dbg(n * factorial(n - 1));
  }
}

int main() {
  std::string message = "hello";
  dbg(message);

  const std::int32_t a = 2;
  const std::int32_t b = dbg(3 * a) + 1;

  std::vector<std::int32_t> numbers = {b, 13, 42};
  dbg(numbers);

  dbg("this line is executed");
  factorial(4);
}
