#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

class Program {
 public:
  Program(const std::vector<std::string>& libraries, const std::string& proxy);

  [[nodiscard]] bool build_pyftsubset() const { return build_pyftsubset_; }
  [[nodiscard]] std::string proxy() const { return proxy_; }

  [[nodiscard]] std::vector<Library>& dependencies() { return dependencies_; }
  [[nodiscard]] std::vector<Library>& libraries_to_be_built() {
    return libraries_to_be_built_;
  }

  void show_libraries();

 private:
  static void unique(std::vector<Library>& libraries);
  static bool contains(const std::vector<Library>& libraries,
                       const std::string& name);

  Library get_from_name(const std::string& name);

  std::string proxy_;

  std::vector<Library> libraries_;
  std::vector<Library> dependencies_;
  std::vector<Library> libraries_to_be_built_;

  bool build_pyftsubset_ = false;
};

}  // namespace kpkg
