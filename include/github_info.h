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

class ReleaseInfo {
 public:
  explicit ReleaseInfo(std::string json);

  [[nodiscard]] const std::string& tag_name() const { return tag_name_; }
  [[nodiscard]] const std::string& url() const { return url_; }
  [[nodiscard]] std::optional<Asset> deb_asset() const;

 private:
  std::string tag_name_;
  std::string url_;

  std::vector<Asset> assets_;
};

class TagInfo {
 public:
  explicit TagInfo(std::string json);

  [[nodiscard]] const std::string& tag_name() const { return tag_name_; }
  [[nodiscard]] const std::string& url() const { return url_; }

 private:
  std::string tag_name_;
  std::string url_;
};

}  // namespace kpkg
