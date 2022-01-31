#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program", "[program]") {
  std::vector<std::string> libraries = {"spdlog", "curl"};
  std::string proxy = "http://127.0.0.1:1080";

  kpkg::Program program(libraries, proxy);

  REQUIRE(program.proxy() == "http://127.0.0.1:1080");

  // TODO

  auto to_be_built = program.libraries_to_be_built();
  REQUIRE(std::size(to_be_built) == 2);
  REQUIRE(to_be_built.front().get_name() == "curl");
  REQUIRE(to_be_built.back().get_name() == "spdlog");
}
