#include "library.h"

#include <filesystem>

#include <klib/archive.h>
#include <klib/log.h>
#include <klib/util.h>
#include <simdjson.h>
#include <spdlog/spdlog.h>
#include <gsl/gsl-lite.hpp>

#include "command.h"
#include "github_info.h"
#include "http.h"

extern char library[];
extern int library_size;

extern char pyftsubset[];
extern int pyftsubset_size;

namespace kpkg {

Library::Library(const std::string& name, const std::string& releases_url,
                 const std::string& tags_url,
                 const std::vector<std::string>& dependency,
                 const std::vector<std::string>& cmd,
                 const std::string& tag_name, const std::string& download_url)
    : name_(name),
      releases_url_(releases_url),
      tags_url_(tags_url),
      dependency_(dependency),
      cmd_(cmd),
      tag_name_(tag_name),
      download_url_(download_url) {}

void Library::init(const std::string& proxy) {
  if (std::empty(tag_name_) && std::empty(download_url_)) {
    std::string url;
    if (!std::empty(releases_url_)) {
      url = releases_url_;
    } else {
      url = tags_url_;
    }
    Ensures(!std::empty(url));

    auto response = http_get(url, proxy);
    if (!std::empty(releases_url_)) {
      ReleaseInfo info(response.text());
      tag_name_ = info.tag_name();
      download_url_ = info.url();
    } else {
      TagInfo info(response.text());
      tag_name_ = info.tag_name();
      download_url_ = info.url();
    }
    Ensures(!std::empty(tag_name_) && !std::empty(download_url_));
  }

  dir_name_ = name_ + "-" + tag_name_;
  file_name_ = dir_name_ + ".tar.gz";
}

void Library::download(const std::string& proxy) const {
  if (std::filesystem::is_regular_file(file_name_)) {
    spdlog::info("Use exists file: {}", file_name_);
  } else {
    spdlog::info("Get file {} from: {}", file_name_, download_url_);

    auto response = http_get(download_url_, proxy);
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
      klib::error("Decompress error");
    }

    spdlog::info("Decompress file: {} to {}", file_name_, *temp);
    spdlog::info("Rename folder from {} to {}", *temp, dir_name_);
    std::filesystem::rename(*temp, dir_name_);
  }

  if (name_ == "pyftsubset") {
    klib::ChangeWorkingDir dir(dir_name_);
    klib::write_file("pyftsubset.py", false, pyftsubset, pyftsubset_size);
  }

  run_commands(cmd_, dir_name_);
}

void Library::print() const { spdlog::info("{:<25} {:<25}", name_, tag_name_); }

std::vector<Library> read_from_json() {
  std::string json(library, library_size);
  json.reserve(std::size(json) + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);

  std::vector<Library> ret;

  for (auto elem : doc.get_array()) {
    std::string name(elem["name"].get_string().value());
    std::string releases_url(elem["releases_url"].get_string().value());
    std::string tags_url(elem["tags_url"].get_string().value());
    std::string tag_name(elem["tag_name"].get_string().value());
    std::string download_url(elem["download_url"].get_string().value());

    std::vector<std::string> dependency;
    for (auto item : elem["dependency"].get_array()) {
      dependency.emplace_back(item.get_string().value());
    }

    std::vector<std::string> cmd;
    for (auto item : elem["cmd"].get_array()) {
      cmd.emplace_back(item.get_string().value());
    }

    ret.emplace_back(name, releases_url, tags_url, dependency, cmd, tag_name,
                     download_url);
  }

  return ret;
}

}  // namespace kpkg
