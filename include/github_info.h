#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace kpkg {

struct Asset {
  Asset(std::string_view name, std::string_view url) : name_(name), url_(url) {}

  std::string name_;
  std::string url_;
};

struct ReleaseInfo {
  std::string tag_name_;
  std::string url_;
  std::vector<Asset> assets_;
};

ReleaseInfo get_release_info(std::string json);

std::optional<Asset> get_deb_asset(const std::vector<Asset> &assets);

struct TagInfo {
  std::string tag_name_;
  std::string url_;
};

TagInfo get_tag_info(std::string json);

}  // namespace kpkg
