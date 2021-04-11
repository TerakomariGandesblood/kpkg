#include <cstddef>
#include <filesystem>

#include <gtest/gtest.h>

#include "decompress.h"
#include "sha.h"

TEST(Decompress, DecompressSingleFile) {
  EXPECT_TRUE(std::filesystem::exists("LICENSE.tar.gz"));
  EXPECT_EQ(
      sha3_512_file("LICENSE.tar.gz"),
      "90cacabc45c51dee2ff3a224028aa6feb05faa1a5adb399bbe482615b4ee79a4b5bd"
      "e4165181c28956bf9d472e311b459d2ba7d2cbeebd35844c2958605e3522");

  auto name = kpkg::decompress("LICENSE.tar.gz");

  EXPECT_EQ(name, "LICENSE");

  EXPECT_TRUE(std::filesystem::exists("LICENSE"));
  EXPECT_EQ(
      sha3_512_file("LICENSE"),
      "f8410ca9108d6d9595445c3f95b0b4ce6a424878f76c26dcf6bbd96518b330c600a6"
      "f06d0ceec51e1aecd1bbd6b6025af4c7c23d147d40905dbf40d9bc07a994");
}

TEST(Decompress, DecompressMultipleFile) {
  EXPECT_TRUE(std::filesystem::exists("zlib-v1.2.11.tar.gz"));
  EXPECT_EQ(
      sha3_512_file("zlib-v1.2.11.tar.gz"),
      "38af19362e48ec80f6565cf18245f520c8ee5348374cb0c11286f3b23cc93fd05a6a"
      "2a2b8784f20bb2307211a2a776241797857b133056f4b33de1d363db7bb2");

  auto name = kpkg::decompress("zlib-v1.2.11.tar.gz");

  EXPECT_EQ(name, "madler-zlib-7085a61");

  EXPECT_TRUE(std::filesystem::exists("madler-zlib-7085a61"));

  std::size_t size = 0;
  for (const auto& item :
       std::filesystem::recursive_directory_iterator("madler-zlib-7085a61")) {
    if (std::filesystem::is_regular_file(item)) {
      size += std::filesystem::file_size(item);
    }
  }
  EXPECT_EQ(size, 2984209);
}
