#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

class Program {
 public:
  Program(const std::vector<std::string>& libraries, const std::string& proxy);

  [[nodiscard]] std::string proxy() const { return proxy_; }

  [[nodiscard]] std::vector<Library>& libraries_to_be_built() {
    return libraries_to_be_built_;
  }

  void show_libraries();

 private:
  static bool contains(std::vector<Library>::const_iterator begin,
                       std::vector<Library>::const_iterator end,
                       const std::string& name);

  Library get_from_name(const std::string& name);
  void add_library(std::vector<Library>& libraries, const Library& library);

  std::string proxy_;

  std::vector<Library> libraries_;
  std::vector<Library> libraries_to_be_built_;
};

}  // namespace kpkg
