#include "library.h"

#include <cassert>
#include <filesystem>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/archive.h>
#include <klib/error.h>
#include <klib/util.h>
#include <simdjson.h>
#include <spdlog/spdlog.h>

#include "command.h"
#include "downloader.h"
#include "github_info.h"
#include "http.h"

extern char library[];
extern int library_size;

extern char font_tools[];
extern int font_tools_size;

namespace kpkg {

Library::Library(const std::string& name, const std::string& releases_url,
                 const std::string& tags_url,
                 const std::vector<std::string>& dependency,
                 const std::string& cwd, const std::vector<std::string>& cmd,
                 const std::vector<std::string>& cmd_install,
                 const std::string& tag_name, const std::string& download_url)
    : name_(name),
      releases_url_(releases_url),
      tags_url_(tags_url),
      dependency_(dependency),
      cwd_(cwd),
      cmd_(cmd),
      cmd_install_(cmd_install),
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
    spdlog::info("Use exists file: {}", file_name_);
  } else {
    spdlog::info("Get file {} from: {}", file_name_, download_url_);

    static HTTPDownloader downloader(proxy);
    downloader.download(download_url_, file_name_);

    spdlog::info("Download file: {} complete", file_name_);
  }
}

void Library::build() const {
  if (std::filesystem::is_directory(dir_name_)) {
    spdlog::info("Use exists folder: {}", dir_name_);
    run_commands(cmd_install_, cwd_);
  } else {
    auto temp = klib::decompress(file_name_);
    if (!temp.has_value()) {
      klib::error(KLIB_CURR_LOC, "Decompress error");
    }

    spdlog::info("Decompress file: {} to {}", file_name_, *temp);
    spdlog::info("Rename folder from {} to {}", *temp, dir_name_);
    std::filesystem::rename(*temp, dir_name_);

    run_commands(cmd_, cwd_);
  }
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

    std::vector<std::string> cmd_install;
    for (auto item : elem["cmd_install"].get_array()) {
      cmd_install.emplace_back(item.get_string().value());
    }

    ret.emplace_back(name, releases_url, tags_url, dependency, cwd, cmd,
                     cmd_install, tag_name, download_url);
  }

  return ret;
}

void build_font_tools(const std::string& proxy) {
  klib::write_file("font_tools.py", false, font_tools, font_tools_size);

  std::vector<std::string> cmd;

  if (!std::empty(proxy)) {
    cmd.push_back(fmt::format(FMT_COMPILE("export all_proxy=\"{}\""), proxy));
  }

  cmd.emplace_back("python3 -m pip install --upgrade pip");
  cmd.emplace_back("python3 -m pip install nuitka fonttools[woff]");
  cmd.emplace_back(
      "python3 -m nuitka --module --include-module=fontTools.subset "
      "--follow-imports --plugin-enable=pylint-warnings --remove-output "
      "--no-pyi-file --lto=yes --prefer-source-code "
      "--python-for-scons=/usr/local/bin/python3 font_tools.py");
  cmd.emplace_back("mv font_tools*.so /usr/local/lib/font_tools.so");

  run_commands(cmd, ".");
}

}  // namespace kpkg
