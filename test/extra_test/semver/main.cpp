#include <semver.hpp>

int main() {
  constexpr semver::version v1("1.2.3");
  constexpr semver::version v2("1.3.1");

  static_assert(v1 < v2);
  static_assert(v1.major == 1);
  static_assert(v1.minor == 2);
  static_assert(v1.patch == 3);
}
