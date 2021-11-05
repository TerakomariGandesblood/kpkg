#include "version.h"

#include <sys/utsname.h>

#include <cerrno>

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/error.h>
#include <klib/version.h>
#include <spdlog/version.h>
#include <boost/version.hpp>
#include <semver.hpp>

namespace kpkg {

std::string version_str() {
  std::string result;

  result += fmt::format(FMT_COMPILE("kpkg version {}\n"), KPKG_VERSION_STRING);

  result += "Libraries: ";
  // TODO use KLIB_VERSION_STRING
  result += fmt::format(FMT_COMPILE("klib/{}.{}.{} "), KLIB_VER_MAJOR,
                        KLIB_VER_MINOR, KLIB_VER_PATCH);
  result += fmt::format(FMT_COMPILE("boost/{}.{}.{} "), BOOST_VERSION / 100000,
                        BOOST_VERSION / 100 % 100, BOOST_VERSION % 100);
  result +=
      fmt::format(FMT_COMPILE("semver/{} "), semver::semver_verion.to_string());
  result += fmt::format(FMT_COMPILE("fmt/{}.{}.{} "), FMT_VERSION / 10000,
                        FMT_VERSION / 100 % 100, FMT_VERSION % 100);
  result += fmt::format(FMT_COMPILE("spdlog/{}.{}.{} "), SPDLOG_VER_MAJOR,
                        SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  // NOTE
  result += "aria2/1.36.0\n";

  result += "Compiler: ";
#if defined(__clang_version__)
  result.append("Clang ").append(__clang_version__);
#elif defined(__GNUG__)
  result.append("GCC ").append(__VERSION__);
#else
  result.append("Unknown compiler");
#endif
  result += "\n";

  result.append("Build time: ")
      .append(__DATE__)
      .append(" ")
      .append(__TIME__)
      .append("\n");

  if (utsname name; !uname(&name)) {
    result += fmt::format(FMT_COMPILE("System: {} {} {} {}"), name.sysname,
                          name.release, name.version, name.machine);
  } else {
    klib::error(std::strerror(errno));
  }

  return result;
}

}  // namespace kpkg
