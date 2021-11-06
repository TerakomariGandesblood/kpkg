#include <iostream>
#include <stdexcept>

#include <scope_guard.hpp>

class Foo {
 public:
  void close() { std::cout << "close()" << std::endl; }
};

int main() {
  try {
    Foo foo;
    // SCOPE_SUCCESS SCOPE_FAIL
    SCOPE_EXIT {
      foo.close();
      std::cout << "[1]" << std::endl;
      // throw std::runtime_error("[2]");
    };

    std::cout << "[3]" << std::endl;

    throw std::runtime_error("[4]");
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
  }
}
