#include "library.h"

#include <cassert>
#include <filesystem>

#include <fmt/format.h>
#include <klib/archive.h>
#include <klib/error.h>
#include <klib/http.h>
#include <spdlog/spdlog.h>

#include "command.h"

extern char library[];
extern int library_size;

namespace kpkg {

Library::Library(const std::string& name, const std::string& releases_url,
                 const std::string& tags_url,
                 const std::vector<std::string>& dependency,
                 const std::string& cwd, const std::vector<std::string>& cmd,
                 const std::string& tag_name, const std::string& download_url)
    : name_(name),
      releases_url_(releases_url),
      tags_url_(tags_url),
      dependency_(dependency),
      cwd_(cwd),
      cmd_(cmd),
      tag_name_(tag_name),
      download_url_(download_url) {}

void Library::init(const std::string& proxy) {
  if (std::empty(tag_name_) && std::empty(download_url_)) {
    std::string url;
    if (!std::empty(releases_url_)) {
      url = releases_url_;
    } else if (!std::empty(tags_url_)) {
      url = tags_url_;
    } else {
      assert(false);
    }
    spdlog::info("Get info from: {} ", url);

    static klib::Request request;
    request.set_browser_user_agent();
    if (!std::empty(proxy)) {
      spdlog::info("Use proxy: {}", proxy);
      request.set_proxy(proxy);
    }
#ifndef NDEBUG
    request.verbose(true);
#endif

    auto response = request.get(url);
    if (response.status_code() != klib::Response::StatusCode::Ok) {
      klib::error("Status code is not ok: {}, url: {}", response.status_code(),
                  url);
    }

    auto result = response.text();
    boost::json::error_code error_code;
    boost::json::monotonic_resource mr;
    auto jv = boost::json::parse(result, error_code, &mr);
    if (error_code) {
      klib::error("Json parse error: {}", error_code.message());
    }

    if (!std::empty(releases_url_)) {
      auto obj = jv.as_object();
      tag_name_ = obj.at("tag_name").as_string().c_str();
      download_url_ = obj.at("tarball_url").as_string().c_str();
    } else if (!std::empty(tags_url_)) {
      auto obj = jv.as_array().front().as_object();
      tag_name_ = obj.at("name").as_string().c_str();
      download_url_ = obj.at("tarball_url").as_string().c_str();
    } else {
      assert(false);
    }
  }

  dir_name_ = name_ + "-" + tag_name_;
  file_name_ = dir_name_ + ".tar.gz";

  if (std::empty(cwd_)) {
    cwd_ = dir_name_;
  } else {
    cwd_ = dir_name_ + "/" + cwd_;
  }
}

void Library::download(const std::string& proxy) const {
  if (std::filesystem::is_regular_file(file_name_)) {
    spdlog::info("Use exists file: {}", file_name_);
  } else {
    spdlog::info("Get file: {} from: {}", file_name_, download_url_);

    static klib::Request request;

    if (!std::empty(proxy)) {
      spdlog::info("Use proxy: {}", proxy);
      request.set_proxy(proxy);
    }
#ifndef NDEBUG
    request.verbose(true);
#endif

    auto response = request.get(download_url_);
    if (response.status_code() != klib::Response::StatusCode::Ok) {
      klib::error("Status code is not ok: {}, url: {}", response.status_code(),
                  download_url_);
    }
    response.save_to_file(file_name_, true);

    spdlog::info("Download file: {} complete", file_name_);
  }
}

void Library::build() const {
  if (std::filesystem::is_directory(dir_name_)) {
    spdlog::info("Use exists folder: {}", dir_name_);
  } else {
    auto temp = klib::decompress(file_name_);
    if (!temp.has_value()) {
      klib::error("decompress error");
    }

    spdlog::info("Decompress file: {}, to {}", file_name_, *temp);
    spdlog::info("Rename folder from {} to {}", *temp, dir_name_);
    std::filesystem::rename(*temp, dir_name_);
  }

  run_commands(cmd_, cwd_);
}

void Library::print() const { fmt::print("{:<25} {:<25}\n", name_, tag_name_); }

std::vector<Library> read_from_json() {
  std::string json_str(library, library_size);

  boost::json::error_code error_code;
  boost::json::monotonic_resource mr;
  auto jv = boost::json::parse(json_str.data(), error_code, &mr);
  if (error_code) {
    klib::error("Json parse error: {}", error_code.message());
  }

  std::vector<Library> ret;
  for (const auto& item : jv.as_array()) {
    ret.push_back(boost::json::value_to<Library>(item));
  }

  return ret;
}

Library tag_invoke(boost::json::value_to_tag<Library>,
                   const boost::json::value& jv) {
  const auto& obj = jv.as_object();
  return Library{value_to<std::string>(obj.at("name")),
                 value_to<std::string>(obj.at("releases_url")),
                 value_to<std::string>(obj.at("tags_url")),
                 value_to<std::vector<std::string>>(obj.at("dependency")),
                 value_to<std::string>(obj.at("cwd")),
                 value_to<std::vector<std::string>>(obj.at("cmd")),
                 value_to<std::string>(obj.at("tag_name")),
                 value_to<std::string>(obj.at("download_url"))};
}

}  // namespace kpkg
