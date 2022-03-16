#include <iostream>
#include <string>

#include <parallel_hashmap/phmap.h>

int main() {
  phmap::flat_hash_map<std::string, std::string> hashmap;
  hashmap.emplace("123", "321");

  for (const auto &[key, value] : hashmap) {
    std::cout << key << ": " << value << '\n';
  }
}
