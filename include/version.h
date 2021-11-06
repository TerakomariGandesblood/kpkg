#pragma once

#include <string>

#define KPKG_VER_MAJOR 0

#define KPKG_VER_MINOR 7

#define KPKG_VER_PATCH 7

#define KPKG_VERSION \
  (KPKG_VER_MAJOR * 10000 + KPKG_VER_MINOR * 100 + KPKG_VER_PATCH)

#define KPKG_STRINGIZE2(s) #s
#define KPKG_STRINGIZE(s) KPKG_STRINGIZE2(s)

#define KPKG_VERSION_STRING      \
  KPKG_STRINGIZE(KPKG_VER_MAJOR) \
  "." KPKG_STRINGIZE(KPKG_VER_MINOR) "." KPKG_STRINGIZE(KPKG_VER_PATCH)

namespace kpkg {

std::string version_str();

}  // namespace kpkg
