#pragma once

#include <string>
#include <vector>

namespace kpkg {

class Library {
 public:
  Library() = default;
  Library(const std::string& name, const std::string& releases_url,
          const std::string& tags_url,
          const std::vector<std::string>& dependency,
          const std::vector<std::string>& cmd, const std::string& tag_name,
          const std::string& download_url);

  void init(const std::string& proxy);

  [[nodiscard]] const std::string& name() const { return name_; }
  [[nodiscard]] const std::vector<std::string>& dependency() const {
    return dependency_;
  }

  void download(const std::string& proxy) const;
  void build() const;

  void print() const;

 private:
  std::string name_;
  std::string releases_url_;
  std::string tags_url_;
  std::vector<std::string> dependency_;
  std::vector<std::string> cmd_;

  std::string tag_name_;
  std::string download_url_;

  std::string file_name_;
  std::string dir_name_;
};

std::vector<Library> read_from_json();

void show_pyftsubset(const std::string& proxy);

void build_pyftsubset(const std::string& proxy);

}  // namespace kpkg
