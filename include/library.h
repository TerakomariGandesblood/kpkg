#pragma once

#include <string>
#include <vector>

#include <boost/json.hpp>

namespace kpkg {

enum class Sanitize { None, Memory, Thread };

class Library {
 public:
  Library() = default;
  Library(const std::string& name, const std::string& author_name,
          const std::string& tag_name, const std::string& releases_url,
          const std::string& tags_url, const std::string& download_url,
          const std::string& cwd, const std::vector<std::string>& cmd);

  void init();

  void download() const;
  void build(Sanitize sanitize = Sanitize::None) const;

  void print() const;

 private:
  std::string name_;
  std::string author_name_;
  std::string tag_name_;
  std::string releases_url_;
  std::string tags_url_;
  std::string download_url_;
  std::string cwd_;
  std::vector<std::string> cmd_;

  std::string file_name_;
  std::string dir_name_;
};

Library tag_invoke(boost::json::value_to_tag<Library>,
                   const boost::json::value& jv);

}  // namespace kpkg
