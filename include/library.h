#pragma once

#include <string>
#include <vector>

#include <boost/json.hpp>

namespace kpkg {

class Library {
 public:
  Library() = default;
  Library(const std::string& name, const std::string& releases_url,
          const std::string& tags_url,
          const std::vector<std::string>& dependency, const std::string& cwd,
          const std::vector<std::string>& cmd, const std::string& tag_name,
          const std::string& download_url);

  void init(bool use_proxy);

  [[nodiscard]] const std::string& get_name() const { return name_; }
  [[nodiscard]] const std::vector<std::string>& get_dependency() const {
    return dependency_;
  }

  void download(bool use_proxy) const;
  void build() const;

  void print() const;

 private:
  std::string name_;
  std::string releases_url_;
  std::string tags_url_;
  std::vector<std::string> dependency_;
  std::string cwd_;
  std::vector<std::string> cmd_;

  std::string tag_name_;
  std::string download_url_;
  std::string file_name_;
  std::string dir_name_;
};

Library tag_invoke(boost::json::value_to_tag<Library>,
                   const boost::json::value& jv);

}  // namespace kpkg
