#include <iostream>
#include <string>

#include <xxhash.h>

int main() {
  std::string str = "Hello World";

  auto status = XXH3_createState();
  XXH3_64bits_reset(status);
  XXH3_64bits_update(status, std::data(str), std::size(str));
  auto hash = XXH3_64bits_digest(status);
  XXH3_freeState(status);

  std::cout << std::hex << hash << '\n';
}
