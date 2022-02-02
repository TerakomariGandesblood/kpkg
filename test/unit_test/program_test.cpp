#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program", "[program]") {
  std::vector<std::string> libraries = {"spdlog", "curl"};
  std::string proxy = "http://127.0.0.1:1080";

  kpkg::Program program(libraries, proxy);

  REQUIRE(program.proxy() == "http://127.0.0.1:1080");

  auto libraries_to_be_built = program.libraries_to_be_built();
  REQUIRE(std::size(libraries_to_be_built) == 11);
  REQUIRE(libraries_to_be_built.front().name() == "fmt");
  REQUIRE(libraries_to_be_built.back().name() == "curl");
}
