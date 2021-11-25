#include <cstdlib>
#include <string>
#include <vector>

#include <klib/error.h>
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include <boost/algorithm/string.hpp>

#include "library.h"
#include "program.h"
#include "upgrade.h"
#include "version.h"

void print_libraries(const std::vector<kpkg::Library>& libraries) {
  if (std::empty(libraries)) {
    return;
  }

  std::vector<std::string> names;
  for (const auto& library : libraries) {
    names.push_back(library.get_name());
  }

  spdlog::info("The following libraries will be installed: {}",
               boost::join(names, " "));
}

void build_libraries(std::vector<kpkg::Library>& libraries,
                     const std::string& proxy) {
  for (auto& item : libraries) {
    item.init(proxy);
    item.download(proxy);
    item.build();
    spdlog::info("{} install complete", item.get_name());
  }
}

int main(int argc, const char* argv[]) try {
  CLI::App app;
  app.set_version_flag("-v,--version", kpkg::version_str());
  app.require_subcommand(1);

  auto install = app.add_subcommand("install", "Install library");

  std::string proxy;
  install
      ->add_flag("-p{http://127.0.0.1:1080},--proxy{http://127.0.0.1:1080}",
                 proxy, "Use proxy")
      ->expected(0, 1);

  std::vector<std::string> libraries;
  install->add_option("library", libraries, "The library to be installed")
      ->required();

  auto list = app.add_subcommand("list", "Show all libraries");
  list->add_flag("-p{http://127.0.0.1:1080},--proxy{http://127.0.0.1:1080}",
                 proxy, "Use proxy")
      ->expected(0, 1);

  auto upgrade = app.add_subcommand("upgrade", "Upgrade kaiser's software");
  upgrade
      ->add_flag("-p{http://127.0.0.1:1080},--proxy{http://127.0.0.1:1080}",
                 proxy, "Use proxy")
      ->expected(0, 1);

  CLI11_PARSE(app, argc, argv);

  kpkg::Program program(libraries, proxy);

  if (!std::empty(proxy)) {
    spdlog::info("Use proxy: {}", proxy);
  }

  if (list->parsed()) {
    program.show_libraries();
    return EXIT_SUCCESS;
  } else if (upgrade->parsed()) {
    kpkg::upgrade(proxy);
    return EXIT_SUCCESS;
  }

  print_libraries(program.dependencies());
  build_libraries(program.dependencies(), program.proxy());

  print_libraries(program.libraries_to_be_built());
  build_libraries(program.libraries_to_be_built(), program.proxy());

  if (program.build_pyftsubset()) {
    spdlog::info("Start building {}", "pyftsubset");
    kpkg::build_pyftsubset();
    spdlog::info("{} install complete", "pyftsubset");
  }
} catch (const std::exception& err) {
  klib::error(KLIB_CURR_LOC, err.what());
} catch (...) {
  klib::error(KLIB_CURR_LOC, "Unknown exception");
}
