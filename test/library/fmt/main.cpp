#include <string>

#include <fmt/format.h>

// https://fmt.dev/latest/index.html
int main() {
  std::string message{fmt::format("The answer is {}", 42)};
  fmt::print("{}\n", message);
  fmt::print(stderr, "{}\n", message);
}
