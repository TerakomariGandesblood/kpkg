#include "upgrade.h"

#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/error.h>
#include <klib/exception.h>
#include <klib/http.h>
#include <klib/util.h>
#include <spdlog/spdlog.h>
#include <boost/json.hpp>
#include <semver.hpp>

#include "downloader.h"

namespace kpkg {

namespace {

std::optional<std::string> get_curr_ver_str(const std::string &name) {
  std::string exec_output;
  try {
    exec_output = klib::exec_with_output("dpkg -s " + name);
  } catch (const klib::RuntimeError &err) {
    return {};
  }

  std::string version_str;
  std::string prefix = "Version: ";
  for (const auto &line : klib::split_str(exec_output, "\n")) {
    if (line.starts_with(prefix)) {
      version_str = line.substr(std::size(prefix));
    }
  }

  return version_str;
}

std::pair<std::string, std::optional<std::string>> get_latest(
    const std::string &name, const std::string &proxy) {
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
    return {tag_name, {}};
  } else {
    return {tag_name,
            assets.at(0).at("browser_download_url").as_string().c_str()};
  }
}

}  // namespace

void upgrade(const std::string &proxy) {
  std::vector<std::string> software = {"klib", "kpkg", "kepub"};
  for (const auto &item : software) {
    auto curr_ver_str = get_curr_ver_str(item);
    if (!curr_ver_str) {
      klib::warn("{} is not installed", item);
      continue;
    }
    auto curr_ver = semver::version(*curr_ver_str);

    auto [latest_tag, download_url] = get_latest(item, proxy);
    auto latest_ver = semver::version(latest_tag.substr(1));

    if (curr_ver < latest_ver) {
      if (!download_url) {
        klib::warn("The {} latest version {} has no assets", item,
                   latest_ver.to_string());
        continue;
      }

      spdlog::info("Will upgrade {} from {} to {}", item, curr_ver.to_string(),
                   latest_ver.to_string());
      spdlog::info("Get file from: {}", *download_url);

      static HTTPDownloader downloader(proxy);
      auto file_name = downloader.download(*download_url);

      spdlog::info("Download file: {} complete", file_name);

      if (std::filesystem::path(file_name).extension() != ".deb") {
        klib::error("Can't find a file in deb format");
      }

      klib::exec("sudo dpkg -i " + file_name);

      if (!std::filesystem::remove(file_name)) {
        klib::error("Remove file {} failed", file_name);
      }

      spdlog::info("{} upgrade completed", item);
    } else {
      spdlog::info("{} is the latest version: {}", item, curr_ver.to_string());
    }
  }
}

}  // namespace kpkg
