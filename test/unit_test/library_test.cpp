#include <catch2/catch.hpp>

#include "library.h"

TEST_CASE("read_from_json") {
  auto libraries = kpkg::read_from_json();
  REQUIRE(std::size(libraries) == 22);
}
