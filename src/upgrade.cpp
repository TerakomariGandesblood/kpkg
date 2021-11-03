#include "upgrade.h"

#include <array>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/error.h>
#include <klib/http.h>
#include <klib/util.h>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>
#include <boost/json.hpp>
#include <semver.hpp>

#include "downloader.h"

namespace kpkg {

namespace {

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po?rq=1
std::string exec(const std::string &cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(pclose) *> pipe(popen(cmd.c_str(), "r"),
                                                 pclose);
  if (!pipe) {
    klib::error("popen() failed");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  return result;
}

std::string get_curr_ver(const std::string &name) {
  if (name == "klib") {
    auto content =
        klib::split_str(exec("cat /usr/include/klib/version.h"), "\n");
    std::string major;
    std::string major_prefix = "#define KLIB_VER_MAJOR ";

    std::string minor;
    std::string minor_prefix = "#define KLIB_VER_MINOR ";

    std::string patch;
    std::string patch_prefix = "#define KLIB_VER_PATCH ";

    for (const auto &line : content) {
      if (line.starts_with(major_prefix)) {
        major = line.substr(std::size(major_prefix));
      } else if (line.starts_with(minor_prefix)) {
        minor = line.substr(std::size(minor_prefix));
      } else if (line.starts_with(patch_prefix)) {
        patch = line.substr(std::size(patch_prefix));
      }
    }

    return fmt::format(FMT_COMPILE("v{}.{}.{}"), major, minor, patch);
  }

  std::string cmd;
  if (name == "kepub") {
    cmd = "sfacg -v";
  } else {
    cmd = name + " -v";
  }

  auto ver_str = exec(cmd);
  return ver_str.substr(ver_str.find_last_of('v'));
}

std::string tidy_ver_str(const std::string &ver_str) {
  auto result = ver_str;
  boost::trim(result);
  return result.substr(1);
}

std::pair<std::string, std::string> get_latest_ver(const std::string &name,
                                                   const std::string &proxy) {
  auto url = fmt::format(
      FMT_COMPILE(
          "https://api.github.com/repos/KaiserLancelot/{}/releases/latest"),
      name);

  klib::Request request;
  request.set_browser_user_agent();
  if (!std::empty(proxy)) {
    request.set_proxy(proxy);
  }
#ifndef NDEBUG
  request.verbose(true);
#endif

  auto response = request.get(url);
  if (!response.ok()) {
    klib::error("Status code is not ok: {}, url: {}", response.status_code(),
                url);
  }

  boost::json::error_code error_code;
  boost::json::monotonic_resource mr;
  auto jv = boost::json::parse(response.text(), error_code, &mr);
  if (error_code) {
    klib::error("Json parse error: {}", error_code.message());
  }

  std::string tag_name = jv.at("tag_name").as_string().c_str();

  auto assets = jv.at("assets").as_array();
  if (assets.empty()) {
    return {tag_name, ""};
  } else {
    return {tag_name,
            assets.at(0).at("browser_download_url").as_string().c_str()};
  }
}

}  // namespace

void upgrade(const std::string &proxy) {
  std::vector<std::string> software;

  if (!std::filesystem::exists("/usr/include/klib/version.h")) {
    klib::warn("klib is not installed");
  } else {
    software.emplace_back("klib");
  }

  if (!std::filesystem::exists("/usr/bin/kpkg")) {
    klib::warn("kpkg is not installed");
  } else {
    software.emplace_back("kpkg");
  }

  if (!std::filesystem::exists("/usr/bin/sfacg")) {
    klib::warn("kepub is not installed");
  } else {
    software.emplace_back("kepub");
  }

  spdlog::info("Use proxy: {}", proxy);
  for (const auto &item : software) {
    auto curr_ver = semver::version(tidy_ver_str(get_curr_ver(item)));
    auto [latest_ver_str, download_url] = get_latest_ver(item, proxy);
    auto latest_ver = semver::version(tidy_ver_str(latest_ver_str));

    if (curr_ver < latest_ver) {
      if (std::empty(download_url)) {
        klib::warn("The {} latest version {} has no assets", item,
                   tidy_ver_str(latest_ver_str));
        continue;
      }

      spdlog::info("Will upgrade {} from {} to {}", item, curr_ver.to_string(),
                   latest_ver.to_string());
      spdlog::info("Get file from: {}", download_url);

      static HTTPDownloader downloader(proxy);
      auto file_name = downloader.download(download_url);

      spdlog::info("Download file: {} complete", file_name);

      if (std::filesystem::path(file_name).extension() != ".deb") {
        klib::error("Can't find a file in deb format");
      }

      klib::execute_command("sudo dpkg -i " + file_name);
      spdlog::info("{} upgrade completed", item);
    } else {
      spdlog::info("{} is the latest version: {}", item, curr_ver.to_string());
    }
  }
}

}  // namespace kpkg
