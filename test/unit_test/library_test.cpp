#include <cstddef>
#include <random>

#include <catch2/catch.hpp>

#include "library.h"

TEST_CASE("read_from_json") {
  auto libraries = kpkg::read_from_json();
  REQUIRE(std::size(libraries) == 22);

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<std::size_t> dis(0, std::size(libraries));

  auto library = libraries[dis(gen)];

  REQUIRE(!std::empty(library.get_name()));

#ifdef KPKG_TEST_USE_PROXY
  library.init("socks5://127.0.0.1:1080");
  library.download("socks5://127.0.0.1:1080");
#endif
}
