#include <catch2/catch.hpp>

#include "version.h"

TEST_CASE("kpkg_version", "[version]") {
  REQUIRE(kpkg::kpkg_version() == "v0.3.5");
}
