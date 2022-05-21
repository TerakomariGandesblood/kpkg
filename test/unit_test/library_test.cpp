#include <cstddef>
#include <random>

#include <catch2/catch_test_macros.hpp>

#include "library.h"

TEST_CASE("read_from_json", "[library]") {
  auto libraries = kpkg::read_from_json();

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<std::size_t> dis(0, std::size(libraries) - 1);

  auto library = libraries[dis(gen)];

  REQUIRE(!std::empty(library.name()));

  std::string proxy;
#ifdef KPKG_TEST_USE_PROXY
  proxy = "http://127.0.0.1:1080";
#endif

  library.init(proxy);
}
