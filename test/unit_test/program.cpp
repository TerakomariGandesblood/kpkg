#include <cstdint>

#include <gtest/gtest.h>

#include "program.h"

TEST(Program, Test1) {
  std::int32_t argc = 5;
  const char* argv[] = {"kpkg", "curl", "zstd", "zlib", "-install", nullptr};

  kpkg::Program program(argc, const_cast<char**>(argv));

  EXPECT_FALSE(program.use_proxy());
  EXPECT_TRUE(program.install_package());
  EXPECT_EQ(program.get_sanitize(), kpkg::Sanitize::None);

  auto install = program.get_package_to_be_install();
  EXPECT_EQ(std::size(install), 17);
  EXPECT_EQ(install.front(), "sudo apt update");
  EXPECT_EQ(install.back(),
            "sudo update-alternatives --install /usr/bin/llvm-symbolizer "
            "llvm-symbolizer /usr/bin/llvm-symbolizer-12 400");

  auto dependency = program.get_dependency();
  EXPECT_EQ(std::size(dependency), 2);
  EXPECT_EQ(dependency.front().get_name(), "openssl");
  EXPECT_EQ(dependency.back().get_name(), "zlib");

  auto to_be_built = program.get_library_to_be_built();
  EXPECT_EQ(std::size(to_be_built), 2);
  EXPECT_EQ(to_be_built.front().get_name(), "curl");
  EXPECT_EQ(to_be_built.back().get_name(), "zstd");
}

TEST(Program, Test2) {
  std::int32_t argc = 4;
  const char* argv[] = {"kpkg", "spdlog", "-proxy", "-memory", nullptr};

  kpkg::Program program(argc, const_cast<char**>(argv));

  EXPECT_TRUE(program.use_proxy());
  EXPECT_FALSE(program.install_package());
  EXPECT_EQ(program.get_sanitize(), kpkg::Sanitize::Memory);

  auto dependency = program.get_dependency();
  EXPECT_EQ(std::size(dependency), 2);
  EXPECT_EQ(dependency.front().get_name(), "libc++");
  EXPECT_EQ(dependency.back().get_name(), "fmt");

  auto to_be_built = program.get_library_to_be_built();
  EXPECT_EQ(std::size(to_be_built), 1);
  EXPECT_EQ(to_be_built.front().get_name(), "spdlog");
}
