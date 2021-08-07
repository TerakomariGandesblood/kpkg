#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>

[[noreturn]] void error(std::string_view msg) {
  std::cerr << "error: " << msg << '\n';
  std::exit(EXIT_FAILURE);
}

[[noreturn]] void require_fail(std::string_view expr,
                               std::string_view file_name,
                               std::uint_least32_t line,
                               std::string_view function_name) {
  std::cerr << "error: " << expr << '\n';
  std::cerr << "at file name: " << file_name
            << "\t function name :" << function_name << "\t line :" << line
            << '\n';

  std::exit(EXIT_FAILURE);
}

#define EXPECT(expr)      \
  (static_cast<bool>(expr) \
       ? void(0)           \
       : require_fail(#expr, __FILE__, __LINE__, __func__))
