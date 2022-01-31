#include "github_info.h"

#include <simdjson.h>

namespace kpkg {

ReleaseInfo::ReleaseInfo(std::string json) {
  json.reserve(std::size(json) + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);

  tag_name_ = doc["tag_name"].get_string().value();
  url_ = doc["tarball_url"].get_string().value();

  for (auto elem : doc["assets"].get_array()) {
    assets_.emplace_back(elem["name"].get_string().value(),
                         elem["browser_download_url"].get_string().value());
  }
}

std::optional<Asset> ReleaseInfo::deb_asset() const {
  for (const auto &asset : assets_) {
    if (asset.name_.ends_with(".deb")) {
      return asset;
    }
  }

  return {};
}

TagInfo::TagInfo(std::string json) {
  json.reserve(std::size(json) + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);

  auto first = doc.at(0);
  tag_name_ = first["name"].get_string().value();
  url_ = first["tarball_url"].get_string().value();
}

}  // namespace kpkg
