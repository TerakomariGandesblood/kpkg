#include <cstddef>
#include <iostream>
#include <string_view>

#include "library_info.h"
#include "port.h"

int main() {
  std::string_view s(port, static_cast<std::size_t>(port_size));
  std::cout << s << '\n';

  kpkg::Curl library(true);
  // library.install();
}
