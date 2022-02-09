#include "library.h"

#include <filesystem>
#include <memory>

#include <klib/archive.h>
#include <klib/log.h>
#include <klib/util.h>
#include <simdjson.h>
#include <boost/core/ignore_unused.hpp>
#include <gsl/gsl-lite.hpp>

#include "command.h"
#include "github_info.h"
#include "http.h"

extern char library[];
extern int library_size;

extern char pyftsubset[];
extern int pyftsubset_size;

extern char libcrypto[];
extern int libcrypto_size;

extern char libssl[];
extern int libssl_size;

extern char openssl[];
extern int openssl_size;

extern char libunistring[];
extern int libunistring_size;

extern char filters[];
extern int filters_size;

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
      auto info = get_release_info(response.text());
      tag_name_ = info.tag_name_;
      download_url_ = info.url_;
    } else {
      auto info = get_tag_info(response.text());
      tag_name_ = info.tag_name_;
      download_url_ = info.url_;
    }
    Ensures(!std::empty(tag_name_) && !std::empty(download_url_));
  }

  dir_name_ = name_ + "-" + tag_name_;
  file_name_ = dir_name_ + ".tar.gz";
}

void Library::download(const std::string& proxy) const {
  if (std::filesystem::is_regular_file(file_name_)) {
    klib::info("Use exists file: {}", file_name_);
  } else {
    klib::info("Get file {} from: {}", file_name_, download_url_);

    auto response = http_get(download_url_, proxy);
    response.save_to_file(file_name_);

    klib::info("Download file: {} complete", file_name_);
  }
}

void Library::build() const {
  if (std::filesystem::is_directory(dir_name_)) {
    klib::info("Use exists folder: {}", dir_name_);
  } else {
    auto temp = klib::outermost_folder_name(file_name_);
    if (!temp.has_value()) {
      klib::error("No outermost folder");
    }
    klib::decompress(file_name_);

    klib::info("Decompress file: {} to {}", file_name_, *temp);
    klib::info("Rename folder from {} to {}", *temp, dir_name_);
    std::filesystem::rename(*temp, dir_name_);
  }

  if (name_ == "pyftsubset") {
    klib::ChangeWorkingDir dir(dir_name_);
    klib::write_file("pyftsubset.py", false, pyftsubset, pyftsubset_size);
  } else if (name_ == "icu") {
    klib::ChangeWorkingDir dir(dir_name_);
    klib::write_file("filters.json", false, filters, filters_size);
  }

  run_commands(cmd_, dir_name_);

  std::unique_ptr<klib::ChangeWorkingDir> dir;
  if (name_ == "libunistring" || name_ == "BoringSSL") {
    klib::info("Start generating pkgconfig file");

    if (!std::filesystem::exists("/usr/local/lib/pkgconfig")) {
      klib::exec("sudo mkdir /usr/local/lib/pkgconfig");
    }

    dir = std::make_unique<klib::ChangeWorkingDir>(dir_name_);
    boost::ignore_unused(dir);
  }

  if (name_ == "libunistring") {
    klib::write_file("libunistring.pc", false, libunistring, libunistring_size);
    klib::exec("sudo cp libunistring.pc /usr/local/lib/pkgconfig");
  } else if (name_ == "BoringSSL") {
    klib::write_file("libcrypto.pc", false, libcrypto, libcrypto_size);
    klib::exec("sudo cp libcrypto.pc /usr/local/lib/pkgconfig");

    klib::write_file("libssl.pc", false, libssl, libssl_size);
    klib::exec("sudo cp libssl.pc /usr/local/lib/pkgconfig");

    klib::write_file("openssl.pc", false, openssl, openssl_size);
    klib::exec("sudo cp openssl.pc /usr/local/lib/pkgconfig");
  }
}

void Library::print() const { klib::info("{:<25} {:<25}", name_, tag_name_); }

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
