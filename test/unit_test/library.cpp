#include <catch2/catch.hpp>

#include "library.h"

TEST_CASE("library") { auto library = kpkg::read_from_port(); }
