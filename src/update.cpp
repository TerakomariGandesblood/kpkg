#include "update.h"

#include <filesystem>
#include <optional>
#include <vector>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/exception.h>
#include <klib/log.h>
#include <klib/util.h>
#include <semver.hpp>

#include "github_info.h"
#include "http.h"

namespace kpkg {

namespace {

std::optional<std::string> get_curr_ver_str(const std::string &name) {
  std::string exec_output;
  try {
    exec_output = klib::pipe("dpkg -s " + name);
  } catch (const klib::RuntimeError &err) {
    return {};
  }

  std::string version_str;
  std::string prefix = "Version: ";
  for (const auto &line : klib::split_str(exec_output, "\n")) {
    if (line.starts_with(prefix)) {
      version_str = line.substr(std::size(prefix));
      break;
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

  auto response = http_get(url, proxy);
  auto info = get_release_info(response.text());

  auto tag_name = info.tag_name_;
  auto asset = get_deb_asset(info.assets_);

  if (asset) {
    return {tag_name, asset->url_};
  } else {
    return {tag_name, {}};
  }
}

}  // namespace

void update(const std::string &proxy) {
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

      klib::info("Will update {} from {} to {}", item, curr_ver.to_string(),
                 latest_ver.to_string());
      klib::info("Get file from: {}", *download_url);

      auto file_name = item + "-" + latest_ver.to_string() + ".deb";
      auto response = http_get(*download_url, proxy);
      response.save_to_file(file_name);

      klib::info("Download file: {} complete", file_name);

      if (std::filesystem::path(file_name).extension() != ".deb") {
        klib::error("Can't find a file in deb format");
      }

      klib::exec("sudo dpkg -i " + file_name);

      if (!std::filesystem::remove(file_name)) {
        klib::error("Remove file {} failed", file_name);
      }

      klib::info("{} update completed", item);
    } else {
      klib::info("{} is the latest version: {}", item, curr_ver.to_string());
    }
  }
}

}  // namespace kpkg
