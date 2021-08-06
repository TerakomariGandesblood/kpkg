#include <cstdint>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program") {
  const char* argv[] = {"0", "install", "spdlog", "curl", "-p", "-i"};
  std::int32_t argc = std::size(argv);

  kpkg::Program program(argc, const_cast<char**>(argv));

  REQUIRE(program.use_proxy());
  REQUIRE(program.install_package());
  REQUIRE(program.get_type() == kpkg::Program::Type::Install);

  auto dependency = program.get_dependency();
  REQUIRE(std::size(dependency) == 4);
  REQUIRE(dependency.at(0).get_name() == "fmt");
  REQUIRE(dependency.at(1).get_name() == "nghttp2");
  REQUIRE(dependency.at(2).get_name() == "openssl");
  REQUIRE(dependency.at(3).get_name() == "zlib");

  auto to_be_built = program.get_library_to_be_built();
  REQUIRE(std::size(to_be_built) == 2);
  REQUIRE(to_be_built.front().get_name() == "curl");
  REQUIRE(to_be_built.back().get_name() == "spdlog");
}
