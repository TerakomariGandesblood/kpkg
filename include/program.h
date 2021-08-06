#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

class Program {
 public:
  Program(std::int32_t argc, const char* argv[]);

  [[nodiscard]] std::string proxy() const { return proxy_; }

  [[nodiscard]] std::vector<Library>& libraries() { return libraries_; }
  [[nodiscard]] std::vector<Library>& dependencies() { return dependencies_; }
  [[nodiscard]] std::vector<Library>& libraries_to_be_built() {
    return libraries_to_be_built_;
  }

 private:
  static void unique(std::vector<Library>& libraries);
  static bool contains(const std::vector<Library>& libraries,
                       const std::string& name);

  void show_libraries();
  Library get_from_name(const std::string& name);

  std::vector<std::string> parse_program_options(std::int32_t argc,
                                                 const char* argv[]);

  std::string proxy_;

  std::vector<Library> libraries_;
  std::vector<Library> dependencies_;
  std::vector<Library> libraries_to_be_built_;
};

}  // namespace kpkg
