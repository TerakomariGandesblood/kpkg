#pragma once

#include <filesystem>
#include <string>

#define KPKG_VER_MAJOR 0

#define KPKG_VER_MINOR 6

#define KPKG_VER_PATCH 6

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define KPKG_VERSION_STRING \
  "v" STRINGIZE(KPKG_VER_MAJOR) "." STRINGIZE(KPKG_VER_MINOR) "." STRINGIZE(KPKG_VER_PATCH)

namespace kpkg {

inline std::string kpkg_version() { return KPKG_VERSION_STRING; }

inline std::string version_str(const std::string &argv0) {
  return std::filesystem::path(argv0).filename().string() +
         " version: " + kpkg_version();
}

}  // namespace kpkg

#undef STRINGIZE2
#undef STRINGIZE
