#include <catch2/catch.hpp>

#include "library.h"

TEST_CASE("library") {
  auto [library, install] = kpkg::read_from_port();

  REQUIRE(std::size(install) == 12);
  REQUIRE(install.front() == "sudo apt update");
  REQUIRE(install.back() ==
          "sudo apt install -y re2c autoconf automake libtool m4 tcl");

  auto curl = kpkg::get_from_name(library, "curl");
  REQUIRE(curl.get_name() == "curl");
  REQUIRE(curl.get_dependency() == std::vector<std::string>{"zlib", "openssl"});
}
