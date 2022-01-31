#include "program.h"

#include <klib/log.h>
#include <boost/algorithm/string.hpp>

namespace kpkg {

Program::Program(const std::vector<std::string>& libraries,
                 const std::string& proxy)
    : proxy_(proxy), libraries_(read_from_json()) {
  for (const auto& library_name : libraries) {
    add_library(libraries_to_be_built_, get_from_name(library_name));
  }

  for (auto iter = std::begin(libraries_to_be_built_) + 1;
       iter != std::end(libraries_to_be_built_);) {
    if (Program::contains(std::begin(libraries_to_be_built_), iter,
                          iter->name())) {
      iter = libraries_to_be_built_.erase(iter);
    } else {
      ++iter;
    }
  }
}

bool Program::contains(std::vector<Library>::const_iterator begin,
                       std::vector<Library>::const_iterator end,
                       const std::string& name) {
  for (auto iter = begin; iter != end; ++iter) {
    if (iter->name() == name) {
      return true;
    }
  }

  return false;
}

void Program::add_library(std::vector<Library>& libraries,
                          const Library& library) {
  for (const auto& item : library.dependency()) {
    add_library(libraries, get_from_name(item));
  }

  libraries.push_back(library);
}

void Program::show_libraries() {
  for (auto& library : libraries_) {
    library.init(proxy_);
    library.print();
  }
}

Library Program::get_from_name(const std::string& name) {
  for (const auto& library : libraries_) {
    if (boost::to_lower_copy(library.name()) == boost::to_lower_copy(name)) {
      return library;
    }
  }

  klib::error("Can not find this library: {}", name);
}

}  // namespace kpkg
