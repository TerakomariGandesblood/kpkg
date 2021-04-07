#include "library_info.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <boost/json.hpp>

#include "decompress.h"
#include "downloader.h"
#include "error.h"

namespace libkpkg {

Library::~Library() {
  if (curl_) {
    curl_easy_cleanup(curl_);
  }

  curl_global_cleanup();
}

Library::Library(bool use_proxy) : use_proxy_(use_proxy) {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  if (curl_ = curl_easy_init(); curl_) {
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1);
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 2);
    curl_easy_setopt(
        curl_, CURLOPT_USERAGENT,
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
        "Gecko) Chrome/91.0.4437.0 Safari/537.36 Edg/91.0.831.1");
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION,
                     Library::callback_func_std_string);

    if (use_proxy_) {
      curl_easy_setopt(curl_, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
    }
  } else {
    error("can not init curl");
  }
}

[[nodiscard]] std::string Library::get_tag() const { return tag_; }

void Library::install() {
  if (!std::filesystem::exists(file_name_)) {
    Downloader downloader(use_proxy_);
    downloader.download(download_url_, file_name_);
  } else {
    spdlog::info("use exists file: {}", file_name_);
  }

  if (!std::filesystem::exists(library_name_ + "-" + tag_)) {
    auto dir = decompress(file_name_);
    std::filesystem::rename(dir, library_name_ + "-" + tag_);
  } else {
    spdlog::info("use exists dir: {}", library_name_);
  }

  command_->run(path_);

  spdlog::info("Build and install {}-{} completed", library_name_, tag_);
}

std::size_t Library::callback_func_std_string(void* contents, std::size_t size,
                                              std::size_t nmemb,
                                              std::string* s) {
  s->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

std::string Library::run_curl(const std::string& url) {
  std::string result;

  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &result);

  auto res{curl_easy_perform(curl_)};
  if (res != CURLE_OK) {
    error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
  }

  return result;
}

Catch2::Catch2(bool use_proxy, Sanitize kind) : Library(use_proxy) {
  command_ = std::make_shared<CMakeCommand>(kind);

  author_name_ = "catchorg";
  library_name_ = "Catch2";

  std::string url = "https://api.github.com/repos/" + author_name_ + "/" +
                    library_name_ + "/releases/latest";

  spdlog::info("get info from: {}", url);
  auto result{run_curl(url)};

  auto jv = boost::json::parse(result);
  tag_ = jv.as_object()["tag_name"].as_string().c_str();
  download_url_ = jv.as_object()["zipball_url"].as_string().c_str();

  dir_name_ = author_name_ + "-" + library_name_ + "-*";
  file_name_ = library_name_ + "-" + tag_ + ".zip";
  path_ = library_name_ + "-" + tag_;

  command_->add_option("CATCH_BUILD_TESTING", "OFF");
  command_->add_option("BUILD_SHARED_LIBS", "TRUE");
}

Doxygen::Doxygen(bool use_proxy, Sanitize kind) : Library(use_proxy) {
  command_ = std::make_shared<CMakeCommand>(kind);

  author_name_ = "doxygen";
  library_name_ = "doxygen";

  std::string url = "https://api.github.com/repos/" + author_name_ + "/" +
                    library_name_ + "/tags";

  spdlog::info("get info from: {}", url);
  auto result{run_curl(url)};

  auto jv = boost::json::parse(result);
  auto latest_tag = jv.as_array().front().as_object();
  tag_ = latest_tag["name"].as_string().c_str();
  download_url_ = latest_tag["zipball_url"].as_string().c_str();

  dir_name_ = author_name_ + "-" + library_name_ + "-*";
  file_name_ = library_name_ + "-" + tag_ + ".zip";
  path_ = library_name_ + "-" + tag_;
}

Boost::Boost(bool use_proxy, Sanitize kind) : Library(use_proxy) {
  command_ = std::make_shared<B2Command>(kind);

  author_name_ = "boostorg";
  library_name_ = "boost";

  std::string url = "https://api.github.com/repos/" + author_name_ + "/" +
                    library_name_ + "/tags";
  spdlog::info("get info from: {}", url);
  auto result{run_curl(url)};

  auto jv = boost::json::parse(result);

  std::size_t i = 0;
  boost::json::object latest_tag;

  do {
    latest_tag = jv.as_array().at(i++).as_object();
    tag_ = latest_tag["name"].as_string().subview(6).data();
  } while (tag_.find("beta") != std::string::npos);

  std::string temp;
  std::replace_copy(std::begin(tag_), std::end(tag_), std::back_inserter(temp),
                    '.', '_');

  download_url_ = fmt::format(
      "https://dl.bintray.com/boostorg/release/{}/source/boost_{}.tar.gz", tag_,
      temp);

  dir_name_ = library_name_ + "_" + temp;
  file_name_ = library_name_ + "-" + tag_ + ".tar.gz";
  path_ = library_name_ + "-" + tag_;
}

ICU::ICU(bool use_proxy, Sanitize kind) : Library(use_proxy) {
  command_ = std::make_shared<MakeCommand>(kind);

  author_name_ = "unicode-org";
  library_name_ = "icu";

  std::string url = "https://api.github.com/repos/" + author_name_ + "/" +
                    library_name_ + "/releases/latest";

  spdlog::info("get info from: {}", url);
  auto result{run_curl(url)};

  auto jv = boost::json::parse(result);
  tag_ = jv.as_object()["tag_name"].as_string().c_str();
  download_url_ = jv.as_object()["zipball_url"].as_string().c_str();

  dir_name_ = author_name_ + "-" + library_name_ + "-*";
  file_name_ = library_name_ + "-" + tag_ + ".zip";
  path_ = library_name_ + "-" + tag_ + "/icu4c/source";

  command_->add_option("enable-tests", "no");
  command_->add_option("enable-samples", "no");
}

Curl::Curl(bool use_proxy, Sanitize kind) : Library(use_proxy) {
  command_ = std::make_shared<CMakeCommand>(kind);

  author_name_ = "curl";
  library_name_ = "curl";

  std::string url = "https://api.github.com/repos/" + author_name_ + "/" +
                    library_name_ + "/releases/latest";

  spdlog::info("get info from: {}", url);
  auto result{run_curl(url)};

  auto jv = boost::json::parse(result);
  tag_ = jv.as_object()["tag_name"].as_string().c_str();
  download_url_ = jv.as_object()["zipball_url"].as_string().c_str();

  dir_name_ = author_name_ + "-" + library_name_ + "-*";
  file_name_ = library_name_ + "-" + tag_ + ".zip";
  path_ = library_name_ + "-" + tag_;

  command_->add_option("BUILD_CURL_EXE", "OFF");
  command_->add_option("BUILD_SHARED_LIBS", "ON");
  command_->add_option("CURL_LTO", "ON");
  command_->add_option("HTTP_ONLY", "ON");
  command_->add_option("ENABLE_MANUAL", "OFF");
  command_->add_option("CURL_ZLIB", "OFF");
  command_->add_option("CURL_ZSTD", "OFF");
  command_->add_option("BUILD_TESTING", "OFF");
}

}  // namespace libkpkg
