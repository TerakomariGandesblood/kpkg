#include <cstdint>
#include <cstdlib>
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

std::string sha_512_string(const std::string& input) {
  std::uint32_t digest_length = SHA512_DIGEST_LENGTH;
  auto digest = static_cast<std::uint8_t*>(std::malloc(digest_length));

  EVP_MD_CTX* context = EVP_MD_CTX_new();
  auto algorithm = EVP_sha512();
  EVP_DigestInit_ex(context, algorithm, nullptr);
  EVP_DigestUpdate(context, input.c_str(), input.size());
  EVP_DigestFinal_ex(context, digest, &digest_length);
  EVP_MD_CTX_destroy(context);

  auto output = bytes_to_hex_string(
      std::vector<std::uint8_t>(digest, digest + digest_length));

  std::free(digest);

  return output;
}

int main() {
  // https://emn178.github.io/online-tools/sha_512.html
  EXPECT(sha_512_string("abc") ==
         "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a21929"
         "92a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");
}
