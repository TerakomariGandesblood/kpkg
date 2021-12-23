#include "program.h"

#include <algorithm>

#include <klib/log.h>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>

namespace kpkg {

Program::Program(const std::vector<std::string>& libraries,
                 const std::string& proxy)
    : proxy_(proxy), libraries_(read_from_json()) {
  for (const auto& library_name : libraries) {
    if (boost::to_lower_copy(library_name) == "pyftsubset") {
      build_pyftsubset_ = true;
      continue;
    }

    auto library = get_from_name(library_name);

    for (const auto& dependency_name : library.get_dependency()) {
      dependencies_.push_back(get_from_name(dependency_name));
    }
    libraries_to_be_built_.push_back(get_from_name(library.get_name()));
  }

  Program::unique(dependencies_);
  Program::unique(libraries_to_be_built_);

  std::erase_if(libraries_to_be_built_, [this](const Library& item) {
    return Program::contains(dependencies_, item.get_name());
  });

  // NOTE
  auto iter = std::find_if(
      std::begin(dependencies_), std::end(dependencies_),
      [](const Library& item) { return item.get_name() == "zlib-ng"; });
  if (iter != std::end(dependencies_)) {
    std::swap(*iter, dependencies_.front());
  }

  iter = std::find_if(
      std::begin(dependencies_), std::end(dependencies_),
      [](const Library& item) { return item.get_name() == "nghttp2"; });
  if (iter != std::end(dependencies_)) {
    std::swap(*iter, dependencies_.back());
  }
}

void Program::unique(std::vector<Library>& libraries) {
  std::sort(std::begin(libraries), std::end(libraries),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });

  libraries.erase(std::unique(std::begin(libraries), std::end(libraries),
                              [](const Library& lhs, const Library& rhs) {
                                return lhs.get_name() == rhs.get_name();
                              }),
                  std::end(libraries));
}

bool Program::contains(const std::vector<Library>& libraries,
                       const std::string& name) {
  for (const auto& library : libraries) {
    if (library.get_name() == name) {
      return true;
    }
  }

  return false;
}

void Program::show_libraries() {
  show_pyftsubset(proxy_);

  for (auto& library : libraries_) {
    library.init(proxy_);
    library.print();
  }
}

Library Program::get_from_name(const std::string& name) {
  for (const auto& library : libraries_) {
    if (boost::to_lower_copy(library.get_name()) ==
        boost::to_lower_copy(name)) {
      return library;
    }
  }

  klib::error("Can not find this library: {}", name);
}

}  // namespace kpkg
