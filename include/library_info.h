#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

#include <curl/curl.h>

#include "command.h"

namespace kpkg {

class Library {
 public:
  explicit Library(bool use_proxy);

  virtual ~Library();

  explicit Library(const Library &) = delete;
  explicit Library(Library &&) = delete;
  Library &operator=(const Library &) = delete;
  Library &operator=(Library &&) = delete;

  [[nodiscard]] std::string get_tag() const;

  void install();

 protected:
  std::string run_curl(const std::string &url);

  std::string author_name_;
  std::string library_name_;

  std::string tag_;
  std::string file_name_;
  std::string dir_name_;

  std::string download_url_;

  std::string path_;

  std::shared_ptr<Command> command_;

 private:
  CURL *curl_;
  bool use_proxy_;

  static std::size_t callback_func_std_string(void *contents, std::size_t size,
                                              std::size_t nmemb,
                                              std::string *s);
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
class Catch2 : public Library {
 public:
  explicit Catch2(bool use_proxy, Sanitize kind = Sanitize::None);
};

class Doxygen : public Library {
 public:
  explicit Doxygen(bool use_proxy, Sanitize kind = Sanitize::None);
};

class Boost : public Library {
 public:
  explicit Boost(bool use_proxy, Sanitize kind = Sanitize::None);
};

class ICU : public Library {
 public:
  explicit ICU(bool use_proxy, Sanitize kind = Sanitize::None);
};

class Curl : public Library {
 public:
  explicit Curl(bool use_proxy, Sanitize kind = Sanitize::None);
};
#pragma clang diagnostic pop

}  // namespace kpkg
