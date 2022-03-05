#include <cstdint>
#include <string>

#include <woff2/encode.h>

#include <klib/log.h>
#include <klib/util.h>

int main() {
  auto data = klib::read_file("SourceHanSansSC-Bold.ttf", true);
  auto data_ptr = reinterpret_cast<const uint8_t *>(std::data(data));
  auto data_size = std::size(data);

  std::string result;
  auto max_size = woff2::MaxWOFF2CompressedSize(data_ptr, data_size);
  result.resize(max_size);

  woff2::WOFF2Params params;
  params.brotli_quality = 6;
  auto rc = woff2::ConvertTTFToWOFF2(
      data_ptr, data_size, reinterpret_cast<uint8_t *>(std::data(result)),
      &max_size, params);
  if (!rc) {
    klib::error("ConvertTTFToWOFF failed");
  }
  result.resize(max_size);

  klib::write_file("SourceHanSansSC-Bold.woff2", true, result);
}
