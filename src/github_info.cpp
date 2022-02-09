#include "github_info.h"

#include <simdjson.h>

namespace kpkg {

ReleaseInfo get_release_info(std::string json) {
  ReleaseInfo result;

  json.reserve(std::size(json) + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);

  result.tag_name_ = doc["tag_name"].get_string().value();
  result.url_ = doc["tarball_url"].get_string().value();

  for (auto elem : doc["assets"].get_array()) {
    result.assets_.emplace_back(
        elem["name"].get_string().value(),
        elem["browser_download_url"].get_string().value());
  }

  return result;
}

std::optional<Asset> get_deb_asset(const std::vector<Asset> &assets) {
  for (const auto &asset : assets) {
    if (asset.name_.ends_with(".deb")) {
      return asset;
    }
  }

  return {};
}

TagInfo get_tag_info(std::string json) {
  TagInfo result;

  json.reserve(std::size(json) + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);

  auto first = doc.at(0);
  result.tag_name_ = first["name"].get_string().value();
  result.url_ = first["tarball_url"].get_string().value();

  return result;
}

}  // namespace kpkg
