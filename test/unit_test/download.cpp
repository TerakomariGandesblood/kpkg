#include <filesystem>

#include <catch2/catch.hpp>

#include "download.h"
#include "sha.h"

#ifdef KPKG_TEST_USE_PROXY
#include "port.h"
#endif

TEST_CASE("download") {
#ifdef KPKG_TEST_USE_PROXY
  kpkg::use_proxy = true;
#endif

  kpkg::get_file(
      "https://github.com/madler/zlib/archive/refs/tags/v1.2.11.tar.gz",
      "zlib-1.2.11.tar.gz");

  REQUIRE(std::filesystem::exists("zlib-1.2.11.tar.gz"));
  // https://emn178.github.io/online-tools/sha3_512_checksum.html
  REQUIRE(sha3_512_file("zlib-1.2.11.tar.gz") ==
          "b623350b319b21cad2b8f4938dbe0c3ae87a33a94a7a065c3471869f150e966f0025"
          "3d4388486e94f0b10669d33cea2b428c09ff2c653392ac713fea8892b472");
}
