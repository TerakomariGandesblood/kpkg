#include "library.h"

#include <filesystem>

#include <klib/archive.h>
#include <klib/log.h>
#include <klib/util.h>
#include <simdjson.h>
#include <gsl/assert>

#include "command.h"
#include "github_info.h"
#include "http.h"

extern char library[];
extern int library_size;

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

extern char boringssl[];
extern int boringssl_size;

extern char woff2[];
extern int woff2_size;

extern char sqlcipher[];
extern int sqlcipher_size;

extern char libidn2[];
extern int libidn2_size;

extern char libspng[];
extern int libspng_size;

extern char libvips[];
extern int libvips_size;

extern char glib[];
extern int glib_size;

namespace kpkg {

namespace {

void write_files(const std::string& library_name, const std::string& dir_name) {
  klib::ChangeWorkingDir dir(dir_name);

  if (library_name == "woff2") {
    klib::write_file("0001-fix-brotli-link.patch", false, woff2, woff2_size);
  } else if (library_name == "BoringSSL") {
    klib::write_file("0001-expose-ripemd160.patch", false, boringssl,
                     boringssl_size);
    klib::write_file("libcrypto.pc", false, libcrypto, libcrypto_size);
    klib::write_file("libssl.pc", false, libssl, libssl_size);
    klib::write_file("openssl.pc", false, openssl, openssl_size);
  } else if (library_name == "sqlcipher") {
    klib::write_file("0001-fix-OPENSSL_VERSION_TEXT.patch", false, sqlcipher,
                     sqlcipher_size);
  } else if (library_name == "icu") {
    klib::write_file("filters.json", false, filters, filters_size);
  } else if (library_name == "libunistring") {
    klib::write_file("libunistring.pc", false, libunistring, libunistring_size);
  } else if (library_name == "libidn2") {
    klib::write_file("0001-remove-HAVE_SYMVER_ALIAS_SUPPORT.patch", false,
                     libidn2, libidn2_size);
  } else if (library_name == "libspng") {
    klib::write_file("libspng.pc", false, libspng, libspng_size);
  } else if (library_name == "libvips") {
    klib::write_file("0001-Add-spngsave.patch", false, libvips, libvips_size);
  } else if (library_name == "glib") {
    klib::write_file("0001-fix-link-error.patch", false, glib, glib_size);
  }
}

}  // namespace

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
  file_name_ = dir_name_ + ".archive";
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
    std::filesystem::remove_all(dir_name_);
  }

  auto temp = klib::outermost_folder_name(file_name_);
  if (!temp.has_value()) {
    klib::error("No outermost folder");
  }
  klib::decompress(file_name_);

  std::filesystem::rename(*temp, dir_name_);

  write_files(name_, dir_name_);

  if (!std::filesystem::exists("/usr/local/lib/pkgconfig")) {
    klib::exec("sudo mkdir /usr/local/lib/pkgconfig");
  }
  run_commands(cmd_, dir_name_);

  klib::exec("sudo rm " + file_name_);
  klib::exec("sudo rm -rf " + dir_name_);
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
