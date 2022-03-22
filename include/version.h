#pragma once

#include <string>

#define KPKG_VER_MAJOR 1

#define KPKG_VER_MINOR 4

#define KPKG_VER_PATCH 2

#define KPKG_STRINGIZE2(s) #s
#define KPKG_STRINGIZE(s) KPKG_STRINGIZE2(s)

#define KPKG_VERSION_STRING      \
  KPKG_STRINGIZE(KPKG_VER_MAJOR) \
  "." KPKG_STRINGIZE(KPKG_VER_MINOR) "." KPKG_STRINGIZE(KPKG_VER_PATCH)

namespace kpkg {

std::string version_str();

}  // namespace kpkg
