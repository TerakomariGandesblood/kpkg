#include <filesystem>

#include <gtest/gtest.h>

#include "download.h"

TEST(Download, Download) {
  bool use_proxy = false;

#ifdef KPKG_TEST_USE_PROXY
  use_proxy = true;
#endif

  kpkg::get_file(
      "https://api.github.com/repos/madler/zlib/tarball/refs/tags/v1.2.11",
      "zlib-1.2.11.tar.gz", use_proxy);

  EXPECT_TRUE(std::filesystem::exists("zlib-1.2.11.tar.gz"));

  EXPECT_EQ(std::filesystem::file_size("zlib-1.2.11.tar.gz"), 644596);
}
