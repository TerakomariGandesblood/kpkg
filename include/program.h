#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <boost/program_options/variables_map.hpp>

#include "library.h"

namespace kpkg {

class Program {
 public:
  Program(std::int32_t argc, char* argv[]);

  void print_dependency() const;

  void print_library_to_be_built() const;

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

 private:
  static boost::program_options::variables_map parse_program_options(
      std::int32_t argc, char* argv[]);

  std::vector<std::string> deal_with_program_options(
      const boost::program_options::variables_map& vm);

  static std::pair<std::vector<Library>, std::vector<std::string>>
  read_from_port();

  static Library get_from_name(const std::vector<Library>& libraries,
                               const std::string& name);

  bool use_proxy_ = false;
  bool install_ = false;

  std::vector<std::string> package_to_be_install_;
  std::vector<Library> libraries_;
  std::vector<Library> dependency_;
  std::vector<Library> library_to_be_built_;
  Sanitize sanitize_ = Sanitize::None;
};

}  // namespace kpkg
