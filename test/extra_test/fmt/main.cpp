#include <cstdint>
#include <iostream>
#include <vector>

#include <fmt/color.h>
#include <fmt/compile.h>  // FMT_COMPILE
#include <fmt/core.h>     // fmt::print
#include <fmt/format.h>   // fmt::format / FMT_STRING
#include <fmt/os.h>       // fmt::output_file
#include <fmt/ostream.h>
#include <fmt/ranges.h>

// https://fmt.dev/latest/index.html
// https://github.com/fmtlib/fmt
int main() {
  // FMT_STRING 编译时检查
  fmt::print(FMT_STRING("The answer is {}\n"), 42);
  fmt::print(stderr, "Don't {}!\n", "panic");

  auto str = fmt::format(FMT_COMPILE("The answer is {}"), 42);
  fmt::print("{}\n", str);

  std::vector<std::int32_t> v = {1, 2, 3};
  fmt::print("{}\n", v);

  fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
             "Elapsed time: {} seconds\n", 1.23);

  fmt::print(std::cerr, "Don't {}!\n", "panic");

  auto out = fmt::output_file("guide.txt");
  out.print("Don't {}!\n", "Panic");

  // TODO Syntax
  // https://fmt.dev/latest/syntax.html
}
