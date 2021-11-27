#include "library.h"

#include <cassert>
#include <filesystem>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/archive.h>
#include <klib/log.h>
#include <klib/util.h>
#include <simdjson.h>

#include "command.h"
#include "downloader.h"
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
    klib::info("Get info from: {} ", url);

    auto response = http_get(url, proxy);
    if (!std::empty(releases_url_)) {
      ReleaseInfo info(response.text());
      tag_name_ = info.get_tag_name();
      download_url_ = info.get_url();
    } else if (!std::empty(tags_url_)) {
      TagInfo info(response.text());
      tag_name_ = info.get_tag_name();
      download_url_ = info.get_url();
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
    klib::info("Use exists file: {}", file_name_);
  } else {
    klib::info("Get file {} from: {}", file_name_, download_url_);

    static HTTPDownloader downloader(proxy);
    downloader.download(download_url_, file_name_);

    klib::info("Download file: {} complete", file_name_);
  }
}

void Library::build() const {
  if (std::filesystem::is_directory(dir_name_)) {
    klib::info("Use exists folder: {}", dir_name_);
  } else {
    auto temp = klib::decompress(file_name_);
    if (!temp.has_value()) {
      klib::error("Decompress error");
    }

    klib::info("Decompress file: {} to {}", file_name_, *temp);
    klib::info("Rename folder from {} to {}", *temp, dir_name_);
    std::filesystem::rename(*temp, dir_name_);
  }

  run_commands(cmd_, cwd_);
}

void Library::print() const {
  fmt::print(FMT_COMPILE("{:<25} {:<25}\n"), name_, tag_name_);
}

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

    std::string cwd(elem["cwd"].get_string().value());

    std::vector<std::string> cmd;
    for (auto item : elem["cmd"].get_array()) {
      cmd.emplace_back(item.get_string().value());
    }

    ret.emplace_back(name, releases_url, tags_url, dependency, cwd, cmd,
                     tag_name, download_url);
  }

  return ret;
}

void show_pyftsubset(const std::string& proxy) {
  auto response = http_get(
      "https://api.github.com/repos/fonttools/fonttools/releases/latest",
      proxy);
  ReleaseInfo info(response.text());

  auto tag_name = info.get_tag_name();
  fmt::print(FMT_COMPILE("{:<25} {:<25}\n"), "pyftsubset", tag_name);
}

void build_pyftsubset() {
  klib::write_file("pyftsubset.py", false, pyftsubset, pyftsubset_size);

  std::vector<std::string> cmd;

  cmd.emplace_back("sudo python3 pyftsubset.py --help");

  cmd.emplace_back(
      "sudo python3 -m nuitka --onefile --plugin-enable=pylint-warnings "
      "--lto=yes --prefer-source-code --static-libpython=yes "
      "--assume-yes-for-downloads -o pyftsubset pyftsubset.py");
  cmd.emplace_back("sudo cp pyftsubset /usr/local/bin/pyftsubset");

  run_commands(cmd, ".");
}

}  // namespace kpkg
