#include <iostream>

#include <nameof.hpp>

enum class Color { Red = 2 };

int main() {
  auto color = Color::Red;

  std::cout << NAMEOF(color) << '\n';
  std::cout << NAMEOF_ENUM(color) << '\n';
}
