#include <cstdint>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program2") {
  std::int32_t argc = 5;
  const char* argv[] = {"kpkg", "spdlog", "curl", "-proxy", "-memory", nullptr};

  kpkg::Program program(argc, const_cast<char**>(argv));

  REQUIRE(program.use_proxy() == true);
  REQUIRE(program.install_package() == false);
  REQUIRE(program.get_sanitize() == kpkg::Sanitize::Memory);

  auto dependency = program.get_dependency();
  REQUIRE(std::size(dependency) == 4);
  REQUIRE(dependency.front().get_name() == "libc++");
  REQUIRE(dependency.at(1).get_name() == "fmt");
  REQUIRE(dependency.at(2).get_name() == "openssl");
  REQUIRE(dependency.back().get_name() == "zlib");

  auto to_be_built = program.get_library_to_be_built();
  REQUIRE(std::size(to_be_built) == 2);
  REQUIRE(to_be_built.front().get_name() == "curl");
  REQUIRE(to_be_built.back().get_name() == "spdlog");
}
