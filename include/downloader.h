#pragma once

#include <string>

#include <aria2/aria2.h>

namespace kpkg {

class HTTPDownloader {
 public:
  explicit HTTPDownloader(const std::string &proxy = "");

  HTTPDownloader(const HTTPDownloader &) = delete;
  HTTPDownloader(HTTPDownloader &&) = delete;
  HTTPDownloader &operator=(const HTTPDownloader &) = delete;
  HTTPDownloader &operator=(HTTPDownloader &&) = delete;

  ~HTTPDownloader();

  void download(const std::string &url, const std::string &file_name);

 private:
  aria2::KeyVals options_;
};

}  // namespace kpkg
