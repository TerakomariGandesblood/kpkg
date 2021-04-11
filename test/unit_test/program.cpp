#include <cstdint>

#include <gtest/gtest.h>

#include "program.h"

TEST(Program, Test) {
  std::int32_t argc = 5;
  const char* argv[] = {"kpkg", "spdlog", "curl", "-proxy", "-memory", nullptr};

  kpkg::Program program(argc, const_cast<char**>(argv));

  EXPECT_TRUE(program.use_proxy());
  EXPECT_FALSE(program.install_package());
  EXPECT_EQ(program.get_sanitize(), kpkg::Sanitize::Memory);

  auto dependency = program.get_dependency();
  EXPECT_EQ(std::size(dependency), 4);
  EXPECT_EQ(dependency.front().get_name(), "libc++");
  EXPECT_EQ(dependency.at(1).get_name(), "fmt");
  EXPECT_EQ(dependency.at(2).get_name(), "openssl");
  EXPECT_EQ(dependency.back().get_name(), "zlib");

  auto to_be_built = program.get_library_to_be_built();
  EXPECT_EQ(std::size(to_be_built), 2);
  EXPECT_EQ(to_be_built.front().get_name(), "curl");
  EXPECT_EQ(to_be_built.back().get_name(), "spdlog");
}
