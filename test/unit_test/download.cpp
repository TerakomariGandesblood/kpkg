#include <filesystem>

#include <catch2/catch.hpp>

#include "download.h"
#include "sha.h"

TEST_CASE("download") {
  bool use_proxy = false;

#ifdef KPKG_TEST_USE_PROXY
  use_proxy = true;
#endif

  kpkg::get_file(
      "https://github.com/madler/zlib/archive/refs/tags/v1.2.11.tar.gz",
      "zlib-1.2.11.tar.gz", use_proxy);

  REQUIRE(std::filesystem::exists("zlib-1.2.11.tar.gz"));

  REQUIRE(std::filesystem::file_size("zlib-1.2.11.tar.gz") == 644492);
}
