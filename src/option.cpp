#include "option.h"

#include <cstddef>
#include <cstdlib>
#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <boost/algorithm/string.hpp>
#include <boost/json.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "error.h"
#include "port.h"
#include "version.h"

namespace kpkg {

std::tuple<std::vector<Library>, std::vector<Library>, Sanitize,
           std::vector<std::string>>
process_option(std::int32_t argc, char* argv[]) {
  boost::program_options::options_description generic("Generic options");
  generic.add_options()("version,v", "print version string")(
      "help,h", "produce help message");

  boost::program_options::options_description config("Configuration");
  config.add_options()("memory,m", "Build with MemorySanitizer")(
      "thread,t", "Build with ThreadSanitizer")("proxy,p", "Use proxy");

  boost::program_options::options_description hidden("Hidden options");
  hidden.add_options()(
      "install-library",
      boost::program_options::value<std::vector<std::string>>(),
      "install library");

  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  boost::program_options::options_description visible("Allowed options");
  visible.add(generic).add(config);

  boost::program_options::positional_options_description p;
  p.add("install-library", -1);

  boost::program_options::variables_map vm;
  store(boost::program_options::command_line_parser(argc, argv)
            .options(cmdline_options)
            .positional(p)
            .run(),
        vm);
  notify(vm);

  if (vm.contains("help")) {
    fmt::print("Usage: {} [options] library...\n\n{}\n", argv[0], visible);
    std::exit(EXIT_SUCCESS);
  }

  if (vm.contains("version")) {
    fmt::print("{} version: {}.{}.{}\n", argv[0], KPKG_VER_MAJOR,
               KPKG_VER_MINOR, KPKG_VER_PATCH);
    std::exit(EXIT_SUCCESS);
  }

  if (!vm.contains("install-library")) {
    kpkg::error("need a library name");
  }

  auto sanitize = Sanitize::None;
  if (vm.contains("memory")) {
    sanitize = Sanitize::Memory;
  } else if (vm.contains("thread")) {
    sanitize = Sanitize::Thread;
  }

  if (vm.contains("proxy")) {
    use_proxy = true;
  }

  auto temp = read_from_port();

  std::vector<Library> pre, lib;

  auto f = [&](const std::string& name) {
    for (const auto& item : temp.first) {
      if (boost::to_lower_copy(item.get_name()) == boost::to_lower_copy(name)) {
        return item;
      }
    }
    error("error");
  };

  for (const auto& item :
       vm["install-library"].as<std::vector<std::string>>()) {
    auto l = f(item);
    auto dep = l.get_dependency();
    for (const auto& i : dep) {
      pre.push_back(f(i));
    }
    lib.push_back(f(l.get_name()));
  }

  if (sanitize != Sanitize::None) {
    pre.insert(std::begin(pre), f("libc++"));
  }

  return {pre, lib, sanitize, temp.second};
}

}  // namespace kpkg
