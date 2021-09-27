#include "program.h"

#include <wait.h>

#include <algorithm>
#include <cstdlib>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <klib/error.h>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "version.h"

namespace kpkg {

Program::Program(std::int32_t argc, const char* argv[])
    : libraries_(read_from_json()) {
  for (const auto& library_name : parse_program_options(argc, argv)) {
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
  // for nghttp2
  auto iter = std::find_if(
      std::begin(dependencies_), std::end(dependencies_),
      [](const Library& item) { return item.get_name() == "nghttp2"; });
  if (iter != std::end(dependencies_)) {
    std::swap(*iter, dependencies_.back());
    std::sort(std::begin(dependencies_), std::end(dependencies_) - 1,
              [](const Library& lhs, const Library& rhs) {
                return lhs.get_name() < rhs.get_name();
              });
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
  auto backup = spdlog::get_level();
  spdlog::set_level(spdlog::level::err);

  for (auto& library : libraries_) {
    library.init(proxy_);
    library.print();
  }

  spdlog::set_level(backup);
}

Library Program::get_from_name(const std::string& name) {
  for (const auto& library : libraries_) {
    if (boost::to_lower_copy(library.get_name()) ==
        boost::to_lower_copy(name)) {
      return library;
    }
  }

  klib::error("can not find this library: {}", name);
}

std::vector<std::string> Program::parse_program_options(std::int32_t argc,
                                                        const char* argv[]) {
  std::vector<std::string> input_libraries;

  std::string help_msg = R"(kpkg list [options]
kpkg install <some library> [options])";

  if (argc == 1) {
    fmt::print("{}", help_msg);
    std::exit(EXIT_SUCCESS);
  } else if (argc > 1) {
    std::string command = argv[1];

    if (command == "list") {
      boost::program_options::options_description generic("Generic options");
      generic.add_options()("version,v", "print version string")(
          "help,h", "produce help message");

      boost::program_options::options_description config("Configuration");
      config.add_options()(
          "proxy,p",
          boost::program_options::value<std::string>(&proxy_)->implicit_value(
              "socks5://127.0.0.1:1080"),
          "Use proxy");

      boost::program_options::options_description visible("Allowed options");
      visible.add(generic).add(config);

      boost::program_options::variables_map vm;
      try {
        store(boost::program_options::command_line_parser(argc - 1, argv + 1)
                  .options(visible)
                  .run(),
              vm);
        notify(vm);
      } catch (const boost::program_options::error& err) {
        klib::error(err.what());
      }

      if (vm.contains("version")) {
        fmt::print("{} version: {}\n", argv[0], kpkg_version());
        std::exit(EXIT_SUCCESS);
      }

      if (vm.contains("help")) {
        fmt::print("{}\n", visible);
        std::exit(EXIT_SUCCESS);
      }

      show_libraries();
      std::exit(EXIT_SUCCESS);
    } else if (command == "install") {
      boost::program_options::options_description generic("Generic options");
      generic.add_options()("version,v", "print version string")(
          "help,h", "produce help message");

      boost::program_options::options_description config("Configuration");
      config.add_options()(
          "proxy,p",
          boost::program_options::value<std::string>(&proxy_)->implicit_value(
              "socks5://127.0.0.1:1080"),
          "Use proxy");

      boost::program_options::options_description hidden("Hidden options");
      hidden.add_options()(
          "input-libraries",
          boost::program_options::value<std::vector<std::string>>(
              &input_libraries));

      boost::program_options::options_description cmdline_options;
      cmdline_options.add(generic).add(config).add(hidden);

      boost::program_options::options_description visible("Allowed options");
      visible.add(generic).add(config);

      boost::program_options::positional_options_description p;
      p.add("input-libraries", -1);

      boost::program_options::variables_map vm;
      try {
        store(boost::program_options::command_line_parser(argc - 1, argv + 1)
                  .options(cmdline_options)
                  .positional(p)
                  .run(),
              vm);
        notify(vm);
      } catch (const boost::program_options::error& err) {
        klib::error(err.what());
      }

      if (vm.contains("version")) {
        fmt::print("{} version: {}\n", argv[0], kpkg_version());
        std::exit(EXIT_SUCCESS);
      }

      if (vm.contains("help")) {
        fmt::print("{}\n", visible);
        std::exit(EXIT_SUCCESS);
      }

      return input_libraries;
    } else if (command == "-v" || command == "--version") {
      fmt::print("{} version: {}\n", argv[0], kpkg_version());
      std::exit(EXIT_SUCCESS);
    } else if (command == "-h" || command == "--help") {
      fmt::print("{}", help_msg);
      std::exit(EXIT_SUCCESS);
    } else {
      klib::error("Unknown command: {}", command);
    }
  }

  return input_libraries;
}

}  // namespace kpkg
