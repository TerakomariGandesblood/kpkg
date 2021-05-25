#include "sha.h"

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
