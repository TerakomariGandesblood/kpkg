#include <openssl/evp.h>
#include <openssl/sha.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

std::string bytes_to_hex_string(const std::vector<uint8_t>& bytes) {
  std::ostringstream stream;
  for (uint8_t b : bytes) {
    stream << std::setw(2) << std::setfill('0') << std::hex
           << static_cast<int>(b);
  }
  return stream.str();
}

std::string sha3_512(const std::string& input) {
  uint32_t digest_length = SHA512_DIGEST_LENGTH;
  const EVP_MD* algorithm = EVP_sha3_512();
  uint8_t* digest = static_cast<uint8_t*>(OPENSSL_malloc(digest_length));
  EVP_MD_CTX* context = EVP_MD_CTX_new();
  EVP_DigestInit_ex(context, algorithm, nullptr);
  EVP_DigestUpdate(context, input.c_str(), input.size());
  EVP_DigestFinal_ex(context, digest, &digest_length);
  EVP_MD_CTX_destroy(context);
  std::string output =
      bytes_to_hex_string(std::vector<uint8_t>(digest, digest + digest_length));
  OPENSSL_free(digest);
  return output;
}

int main() {
  std::string output = sha3_512(std::string("abcdef"));
  std::cout << output << "\n";
}
