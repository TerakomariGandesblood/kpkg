#include <catch2/catch.hpp>

#include "version.h"

TEST_CASE("klib_version") { REQUIRE(kpkg::kpkg_version() == "v0.3.0"); }
