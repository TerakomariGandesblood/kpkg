#include <cstdint>

#include <catch2/catch.hpp>

#include "program.h"

TEST_CASE("program") {
  std::int32_t argc = 5;
  const char* argv[] = {"kpkg", "curl", "zstd", "zlib", "-i", nullptr};

  kpkg::Program program(argc, argv);

  REQUIRE(program.use_proxy() == false);
  REQUIRE(program.install_package() == true);
  REQUIRE(program.get_sanitize() == kpkg::Sanitize::None);

  auto install = program.get_package_to_be_install();
  REQUIRE(std::size(install) == 17);
  REQUIRE(install.front() == "sudo apt update");
  REQUIRE(install.back() ==
          "sudo update-alternatives --install /usr/bin/llvm-symbolizer "
          "llvm-symbolizer /usr/bin/llvm-symbolizer-12 400");

  auto dependency = program.get_dependency();
  REQUIRE(std::size(dependency) == 2);
  REQUIRE(dependency.front().get_name() == "openssl");
  REQUIRE(dependency.back().get_name() == "zlib");

  auto to_be_built = program.get_library_to_be_built();
  REQUIRE(std::size(to_be_built) == 2);
  REQUIRE(to_be_built.front().get_name() == "curl");
  REQUIRE(to_be_built.back().get_name() == "zstd");
}

TEST_CASE("program2") {
  std::int32_t argc = 4;
  const char* argv[] = {"kpkg", "spdlog", "-p", "-m", nullptr};

  kpkg::Program program(argc, argv);

  REQUIRE(program.use_proxy() == true);
  REQUIRE(program.install_package() == false);
  REQUIRE(program.get_sanitize() == kpkg::Sanitize::Memory);

  auto dependency = program.get_dependency();
  REQUIRE(std::size(dependency) == 2);
  REQUIRE(dependency.front().get_name() == "libc++");
  REQUIRE(dependency.back().get_name() == "fmt");

  auto to_be_built = program.get_library_to_be_built();
  REQUIRE(std::size(to_be_built) == 1);
  REQUIRE(to_be_built.front().get_name() == "spdlog");
}
