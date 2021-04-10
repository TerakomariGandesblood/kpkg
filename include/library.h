#pragma once

#include <string>
#include <utility>
#include <vector>

#include <boost/json.hpp>

namespace kpkg {

enum class Sanitize { None, Memory, Thread };

class Library {
 public:
  Library() = default;
  Library(const std::string& name, const std::string& releases_url,
          const std::string& tags_url,
          const std::vector<std::string>& dependency, const std::string& cwd,
          const std::vector<std::string>& cmd, const std::string& tag_name,
          const std::string& download_url);

  void init();

  [[nodiscard]] const std::string& get_name() const { return name_; }
  [[nodiscard]] const std::vector<std::string>& get_dependency() const {
    return dependency_;
  }

  void download() const;
  void build(Sanitize sanitize = Sanitize::None) const;

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

std::pair<std::vector<Library>, std::vector<std::string>> read_from_port();

Library get_from_name(const std::vector<Library>& libraries,
                      const std::string& name);

}  // namespace kpkg
