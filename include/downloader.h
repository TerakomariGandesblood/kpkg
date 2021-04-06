#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include <curl/curl.h>

namespace libkpkg {

class Downloader {
 public:
  explicit Downloader(bool use_proxy);
  ~Downloader();

  explicit Downloader(const Downloader &) = delete;
  explicit Downloader(Downloader &&) = delete;
  Downloader &operator=(const Downloader &) = delete;
  Downloader &operator=(Downloader &&) = delete;

  bool download(const std::string &url, const std::string &file_name);

 private:
  void wait(std::int32_t ms) const;
  static std::size_t write_data(void *ptr, std::size_t size, std::size_t nmemb,
                                void *stream);

  CURL *http_handle_{};
  CURLM *multi_handle_{};
};

}  // namespace libkpkg
