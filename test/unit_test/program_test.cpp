#include <cstdint>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program") {
  const char* argv[] = {"kpkg", "install", "spdlog", "curl", "-p"};
  std::int32_t argc = std::size(argv);

  kpkg::Program program(argc, argv);

  REQUIRE(program.proxy() == "socks5://127.0.0.1:1080");

  auto dependency = program.dependencies();
  REQUIRE(std::size(dependency) == 4);
  REQUIRE(dependency.at(0).get_name() == "fmt");
  REQUIRE(dependency.at(1).get_name() == "openssl");
  REQUIRE(dependency.at(2).get_name() == "zlib");
  REQUIRE(dependency.at(3).get_name() == "nghttp2");

  auto to_be_built = program.libraries_to_be_built();
  REQUIRE(std::size(to_be_built) == 2);
  REQUIRE(to_be_built.front().get_name() == "curl");
  REQUIRE(to_be_built.back().get_name() == "spdlog");
}
