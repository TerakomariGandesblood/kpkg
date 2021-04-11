#include "program.h"

#include <algorithm>
#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gflags/gflags.h>
#include <boost/algorithm/string.hpp>

#include "error.h"
#include "version.h"

extern char port[];
extern int port_size;

DEFINE_bool(memory, false, "Build with MemorySanitizer");
DEFINE_bool(thread, false, "Build with ThreadSanitizer");
DEFINE_bool(proxy, false, "Use proxy");
DEFINE_bool(install, false, "Install package");

namespace kpkg {

Program::Program(std::int32_t argc, char** argv) {
  auto input_library = parse_program_options(argc, argv);

  std::tie(libraries_, package_to_be_install_) = read_from_port();

  for (const auto& item : input_library) {
    auto library = get_from_name(libraries_, item);
    auto dep = library.get_dependency();
    for (const auto& i : dep) {
      dependency_.push_back(get_from_name(libraries_, i));
    }
    library_to_be_built_.push_back(
        get_from_name(libraries_, library.get_name()));
  }

  if (sanitize_ != Sanitize::None) {
    dependency_.insert(std::begin(dependency_),
                       get_from_name(libraries_, "libc++"));
  }

  std::sort(std::begin(libraries_), std::end(libraries_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });

  std::sort(std::begin(dependency_), std::end(dependency_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  dependency_.erase(std::unique(std::begin(dependency_), std::end(dependency_),
                                [](const Library& lhs, const Library& rhs) {
                                  return lhs.get_name() == rhs.get_name();
                                }),
                    std::end(dependency_));

  std::sort(std::begin(library_to_be_built_), std::end(library_to_be_built_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  library_to_be_built_.erase(
      std::unique(std::begin(library_to_be_built_),
                  std::end(library_to_be_built_),
                  [](const Library& lhs, const Library& rhs) {
                    return lhs.get_name() == rhs.get_name();
                  }),
      std::end(library_to_be_built_));

  for (auto iter = std::begin(library_to_be_built_);
       iter != std::end(library_to_be_built_);) {
    if (std::find_if(std::begin(dependency_), std::end(dependency_),
                     [iter](const Library& item) {
                       return item.get_name() == iter->get_name();
                     }) != std::end(dependency_)) {
      iter = library_to_be_built_.erase(iter);
    } else {
      ++iter;
    }
  }

  for (auto iter = std::begin(dependency_); iter != std::end(dependency_);
       ++iter) {
    if (iter->get_name() == "libc++") {
      std::swap(*iter, dependency_.front());
      break;
    }
  }
}

void Program::print_dependency() const {
  if (std::empty(dependency_)) {
    return;
  }

  fmt::print("The following dependent libraries will be installed: ");

  for (const auto& item : dependency_) {
    fmt::print("{} ", item.get_name());
  }

  fmt::print("\n");
}

void Program::print_library_to_be_built() const {
  if (std::empty(library_to_be_built_)) {
    error("library_to_be_built_ is empty");
  }

  fmt::print("The following libraries will be installed: ");

  for (const auto& item : library_to_be_built_) {
    fmt::print("{} ", item.get_name());
  }

  fmt::print("\n");
}

std::vector<std::string> Program::parse_program_options(std::int32_t argc,
                                                        char* argv[]) {
  gflags::SetUsageMessage(fmt::format(R"(

Usage: {} [options] library ...

Allowed options:

    -version             print version string
    -help                produce help message
    -memory              Build with MemorySanitizer
    -thread              Build with ThreadSanitizer
    -proxy               Use proxy
    -install             Install package)",
                                      argv[0]));
  gflags::SetVersionString(
      fmt::format("{}.{}.{}", KPKG_VER_MAJOR, KPKG_VER_MINOR, KPKG_VER_PATCH));
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_proxy) {
    use_proxy_ = true;
  }
  if (FLAGS_install) {
    install_ = true;
  }

  if (FLAGS_memory) {
    sanitize_ = Sanitize::Memory;
  } else if (FLAGS_thread) {
    sanitize_ = Sanitize::Thread;
  }

  if (argc == 1) {
    kpkg::error("need a library name");
  }

  std::vector<std::string> result;
  for (std::int32_t i = 1; i < argc; ++i) {
    result.emplace_back(argv[i]);
  }

  return result;
}

std::pair<std::vector<Library>, std::vector<std::string>>
Program::read_from_port() {
  std::string s(port, static_cast<std::size_t>(port_size));

  boost::json::error_code error_code;
  auto jv = boost::json::parse(s.data(), error_code, {});
  if (error_code) {
    error("json parse error");
  }

  jv = jv.as_object();

  std::vector<std::string> install;
  auto arr = jv.at("install").as_array();
  for (const auto& item : arr) {
    install.emplace_back(item.as_string().c_str());
  }

  auto ports = jv.at("port").as_array();
  std::vector<Library> ret;

  for (const auto& item : ports) {
    ret.push_back(boost::json::value_to<Library>(item));
  }

  return {ret, install};
}

Library Program::get_from_name(const std::vector<Library>& libraries,
                               const std::string& name) {
  for (const auto& item : libraries) {
    if (boost::to_lower_copy(item.get_name()) == boost::to_lower_copy(name)) {
      return item;
    }
  }
  error("can not find this library: {}", name);
}

}  // namespace kpkg
