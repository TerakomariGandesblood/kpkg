#include "version.h"

#include <fmt/compile.h>
#include <fmt/format.h>
#include <klib/version.h>
#include <mimalloc.h>
#include <simdjson.h>
#include <CLI/Version.hpp>
#include <boost/version.hpp>
#include <semver.hpp>

namespace kpkg {

std::string version_str() {
  std::string result;

  result += fmt::format(FMT_COMPILE("kpkg version {}\n"), KPKG_VERSION_STRING);

  result += "Libraries: ";
  result += fmt::format(FMT_COMPILE("klib/{} "), KLIB_VERSION_STRING);
  result +=
      fmt::format(FMT_COMPILE("mimalloc/{}.{}.{} "), MI_MALLOC_VERSION / 100,
                  MI_MALLOC_VERSION / 10 % 10, MI_MALLOC_VERSION % 10);
  result += fmt::format("RE2/2022.04.01 ");
  result += fmt::format(FMT_COMPILE("Boost/{}.{}.{} "), BOOST_VERSION / 100000,
                        BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
  // NOTE
  result += fmt::format("GSL/4.0.0 ");
  result += fmt::format(FMT_COMPILE("CLI11/{} "), CLI11_VERSION);
  result += fmt::format(FMT_COMPILE("fmt/{}.{}.{} "), FMT_VERSION / 10000,
                        FMT_VERSION / 100 % 100, FMT_VERSION % 100);
  result += fmt::format(
      FMT_COMPILE("simdjson/{}.{}.{} "), simdjson::SIMDJSON_VERSION_MAJOR,
      simdjson::SIMDJSON_VERSION_MINOR, simdjson::SIMDJSON_VERSION_REVISION);
  result += fmt::format(FMT_COMPILE("semver/{}\n"),
                        semver::semver_verion.to_string());

  result += "Compiler: ";
#if defined(__clang_version__)
  result.append("Clang ").append(__clang_version__);
#elif defined(__GNUG__)
  result.append("GCC ").append(__VERSION__);
#else
  result.append("Unknown compiler");
#endif
  result += "\n";

  result.append("Build time: ").append(__DATE__).append(" ").append(__TIME__);

  return result;
}

}  // namespace kpkg
