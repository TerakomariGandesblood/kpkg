#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

class Program {
 public:
  enum class Type { Install, List };

  Program(std::int32_t argc, char* argv[]);

  void print_dependency() const;

  void print_library_to_be_built() const;

  [[nodiscard]] std::vector<Library>& get_libraries() { return libraries_; }

  [[nodiscard]] bool use_proxy() const { return use_proxy_; }

  [[nodiscard]] bool install_package() const { return install_; }

  [[nodiscard]] const std::vector<std::string>& get_package_to_be_install()
      const {
    return package_to_be_install_;
  }

  [[nodiscard]] std::vector<Library>& get_dependency() { return dependency_; }

  [[nodiscard]] std::vector<Library>& get_library_to_be_built() {
    return library_to_be_built_;
  }

  [[nodiscard]] Sanitize get_sanitize() const { return sanitize_; }

  [[nodiscard]] Type get_type() const { return type_; }

 private:
  std::vector<std::string> parse_program_options(std::int32_t argc,
                                                 char* argv[]);

  static std::pair<std::vector<Library>, std::vector<std::string>>
  read_from_port();

  static Library get_from_name(const std::vector<Library>& libraries,
                               const std::string& name);

  bool use_proxy_ = false;
  bool install_ = false;
  Type type_;

  std::vector<std::string> package_to_be_install_;
  std::vector<Library> libraries_;
  std::vector<Library> dependency_;
  std::vector<Library> library_to_be_built_;
  Sanitize sanitize_ = Sanitize::None;
};

}  // namespace kpkg
