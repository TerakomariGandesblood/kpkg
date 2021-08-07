#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "error.h"

// https://stackoverflow.com/questions/51144505/generate-sha-3-hash-in-c-using-openssl-library
std::string bytes_to_hex_string(const std::vector<std::uint8_t>& bytes) {
  std::string str;
  for (auto byte : bytes) {
    str += fmt::format(FMT_COMPILE("{:02x}"), static_cast<int>(byte));
  }

  return str;
}

std::string sha3_512_string(const std::string& input) {
  std::uint32_t digest_length = SHA512_DIGEST_LENGTH;
  auto digest = static_cast<uint8_t*>(OPENSSL_malloc(digest_length));

  EVP_MD_CTX* context = EVP_MD_CTX_new();
  auto algorithm = EVP_sha3_512();
  EVP_DigestInit_ex(context, algorithm, nullptr);
  EVP_DigestUpdate(context, input.c_str(), input.size());
  EVP_DigestFinal_ex(context, digest, &digest_length);
  EVP_MD_CTX_destroy(context);

  auto output = bytes_to_hex_string(
      std::vector<std::uint8_t>(digest, digest + digest_length));

  OPENSSL_free(digest);

  return output;
}

std::string sha3_512_file(const std::string& file) {
  std::ifstream ifs{file, std::ifstream::binary};
  if (!ifs) {
    std::cerr << "can not open file: " << file << '\n';
    std::exit(EXIT_FAILURE);
  }

  std::string data;
  data.resize(static_cast<std::string::size_type>(
      ifs.seekg(0, std::ifstream::end).tellg()));
  ifs.seekg(0, std::ifstream::beg)
      .read(data.data(), static_cast<std::streamsize>(std::size(data)));

  return sha3_512_string(data);
}

int main() {
  // https://emn178.github.io/online-tools/sha3_512.html
  EXPECT(sha3_512_string("abc") ==
         "b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e10e11"
         "6e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0");

  EXPECT(sha3_512_file("zlib-v1.2.11.tar.gz") ==
         "38af19362e48ec80f6565cf18245f520c8ee5348374cb0c11286f3b23cc93fd05a6a"
         "2a2b8784f20bb2307211a2a776241797857b133056f4b33de1d363db7bb2");
}
