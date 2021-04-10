#include "library.h"

#include <cstddef>
#include <filesystem>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "command.h"
#include "decompress.h"
#include "download.h"
#include "error.h"

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

void Library::init(bool use_proxy) {
  boost::json::error_code error_code;
  boost::json::monotonic_resource mr;

  if (std::empty(tag_name_) && std::empty(download_url_)) {
    if (!std::empty(releases_url_)) {
      spdlog::info("get info from: {} ", releases_url_);

      auto result = get_page(releases_url_, use_proxy);
      auto jv = boost::json::parse(result, error_code, &mr);
      if (error_code) {
        error("json parse error");
      }

      if (jv.is_object() && jv.as_object().contains("message")) {
        error("{}: {}\ndoc: {}", name_,
              jv.as_object().at("message").as_string().c_str(),
              jv.as_object().at("documentation_url").as_string().c_str());
      }

      auto obj = jv.as_object();
      tag_name_ = obj.at("tag_name").as_string().c_str();
      download_url_ = obj.at("tarball_url").as_string().c_str();
    } else if (!std::empty(tags_url_)) {
      spdlog::info("get info from: {} ", tags_url_);

      auto result = get_page(tags_url_, use_proxy);
      auto jv = boost::json::parse(result, error_code, &mr);

      if (error_code) {
        error("json parse error");
      }

      if (jv.is_object() && jv.as_object().contains("message")) {
        error("{}: {}\ndoc: {}", name_,
              jv.as_object().at("message").as_string().c_str(),
              jv.as_object().at("documentation_url").as_string().c_str());
      }

      auto obj = jv.as_array().front().as_object();
      tag_name_ = obj.at("name").as_string().c_str();
      download_url_ = obj.at("tarball_url").as_string().c_str();
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

void Library::download(bool use_proxy) const {
  if (std::filesystem::exists(file_name_)) {
    spdlog::info("use exists file: {}", file_name_);
  } else {
    spdlog::info("get file: {} from: {}", file_name_, download_url_);
    get_file(download_url_, file_name_, use_proxy);
    spdlog::info("download file: {} complete", file_name_);
  }
}

void Library::build(Sanitize sanitize) const {
  if (std::filesystem::exists(dir_name_)) {
    spdlog::info("use exists folder: {}", dir_name_);
  } else {
    auto temp = decompress(file_name_);
    spdlog::info("decompress file: {}, to {}", file_name_, temp);

    spdlog::info("rename folder from {} to {}", temp, dir_name_);
    std::filesystem::rename(temp, dir_name_);
  }

  run_cmds(cmd_, cwd_, sanitize);
}

void Library::print() const { fmt::print("{}\t\t\t{}\n", name_, tag_name_); }

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
