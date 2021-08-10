#pragma once

#include <string>

#define KPKG_VER_MAJOR 0

#define KPKG_VER_MINOR 3

#define KPKG_VER_PATCH 9

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define KPKG_VERSION_STRING \
  "v" STRINGIZE(KPKG_VER_MAJOR) "." STRINGIZE(KPKG_VER_MINOR) "." STRINGIZE(KPKG_VER_PATCH)

namespace kpkg {

inline std::string kpkg_version() { return KPKG_VERSION_STRING; }

}  // namespace kpkg

#undef STRINGIZE2
#undef STRINGIZE
