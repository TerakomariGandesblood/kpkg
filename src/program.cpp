#include "program.h"

#include <algorithm>
#include <cstdlib>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "error.h"
#include "version.h"

extern char port[];
extern int port_size;

namespace kpkg {

Program::Program(std::int32_t argc, char* argv[]) {
  auto input_library = parse_program_options(argc, argv);

  std::tie(libraries_, package_to_be_install_) = read_from_port();

  if (type_ == Type::List) {
    return;
  }

  for (const auto& item : input_library) {
    auto library = get_from_name(libraries_, item);
    auto dep = library.get_dependency();
    for (const auto& i : dep) {
      dependency_.push_back(get_from_name(libraries_, i));
    }
    library_to_be_built_.push_back(
        get_from_name(libraries_, library.get_name()));
  }

  std::sort(std::begin(libraries_), std::end(libraries_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });

  std::sort(std::begin(dependency_), std::end(dependency_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  dependency_.erase(std::unique(std::begin(dependency_), std::end(dependency_),
                                [](const Library& lhs, const Library& rhs) {
                                  return lhs.get_name() == rhs.get_name();
                                }),
                    std::end(dependency_));

  std::sort(std::begin(library_to_be_built_), std::end(library_to_be_built_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  library_to_be_built_.erase(
      std::unique(std::begin(library_to_be_built_),
                  std::end(library_to_be_built_),
                  [](const Library& lhs, const Library& rhs) {
                    return lhs.get_name() == rhs.get_name();
                  }),
      std::end(library_to_be_built_));

  for (auto iter = std::begin(library_to_be_built_);
       iter != std::end(library_to_be_built_);) {
    if (std::find_if(std::begin(dependency_), std::end(dependency_),
                     [iter](const Library& item) {
                       return item.get_name() == iter->get_name();
                     }) != std::end(dependency_)) {
      iter = library_to_be_built_.erase(iter);
    } else {
      ++iter;
    }
  }

  for (auto iter = std::begin(dependency_); iter != std::end(dependency_);
       ++iter) {
    if (iter->get_name() == "libc++") {
      std::swap(*iter, dependency_.front());
      break;
    }
  }
}

void Program::print_dependency() const {
  if (std::empty(dependency_)) {
    return;
  }

  spdlog::debug("The following dependent libraries will be installed: ");

  for (const auto& item : dependency_) {
    spdlog::debug("{}", item.get_name());
  }
}

void Program::print_library_to_be_built() const {
  if (std::empty(library_to_be_built_)) {
    error("library to be built is empty");
  }

  spdlog::debug("The following libraries will be installed: ");

  for (const auto& item : library_to_be_built_) {
    spdlog::debug("{}", item.get_name());
  }
}

std::vector<std::string> Program::parse_program_options(std::int32_t argc,
                                                        char* argv[]) {
  try {
    std::string command;

    boost::program_options::options_description generic("Generic options");
    generic.add_options()("version,v", "print version string");

    boost::program_options::options_description config("Configuration");
    config.add_options()("proxy,p", "Use proxy");

    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()("command",
                         boost::program_options::value<std::string>(&command))(
        "sub_args", boost::program_options::value<std::vector<std::string>>());

    boost::program_options::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    boost::program_options::options_description visible("Allowed options");
    visible.add(generic).add(config);

    boost::program_options::positional_options_description p;
    p.add("command", 1).add("sub_args", -1);

    boost::program_options::variables_map vm;
    auto parsed = boost::program_options::command_line_parser(argc, argv)
                      .options(cmdline_options)
                      .positional(p)
                      .allow_unregistered()
                      .run();
    store(parsed, vm);
    notify(vm);

    if (vm.contains("version")) {
      fmt::print("{} version: {}.{}.{}\n", argv[0], KPKG_VER_MAJOR,
                 KPKG_VER_MINOR, KPKG_VER_PATCH);
      std::exit(EXIT_SUCCESS);
    }

    if (vm.contains("proxy")) {
      use_proxy_ = true;
    }

    if (command == "install") {
      type_ = Type::Install;

      boost::program_options::options_description install_config(
          "Install configuration");
      install_config.add_options()("install,i", "Install package")(
          "memory,m", "Use memory")("thread,t", "Use thread");

      boost::program_options::options_description install_hidden(
          "Hidden options");
      install_hidden.add_options()(
          "install-libraries",
          boost::program_options::value<std::vector<std::string>>());

      boost::program_options::options_description install_cmdline_options;
      install_cmdline_options.add(install_config).add(install_hidden);

      boost::program_options::options_description install_visible(
          "Allowed options");
      install_visible.add(install_config);

      boost::program_options::positional_options_description install_pos;
      install_pos.add("install-libraries", -1);

      auto opts = boost::program_options::collect_unrecognized(
          parsed.options, boost::program_options::include_positional);
      opts.erase(opts.begin());

      boost::program_options::store(
          boost::program_options::command_line_parser(opts)
              .options(install_cmdline_options)
              .positional(install_pos)
              .run(),
          vm);
      notify(vm);

      if (vm.contains("install")) {
        install_ = true;
      }

      if (!vm.contains("install-libraries")) {
        error("need a library name");
      }

      return vm["install-libraries"].as<std::vector<std::string>>();
    } else if (command == "list") {
      type_ = Type::List;
      return {};
    } else if (std::empty(command)) {
      error("need a library name");
    } else {
      throw boost::program_options::invalid_option_value(command);
    }
  } catch (const boost::program_options::error& err) {
    error(err.what());
  }
}

std::pair<std::vector<Library>, std::vector<std::string>>
Program::read_from_port() {
  std::string s(port, static_cast<std::size_t>(port_size));

  boost::json::error_code error_code;
  boost::json::monotonic_resource mr;
  boost::json::parse_options options;
  options.allow_comments = true;
  auto jv = boost::json::parse(s.data(), error_code, &mr, options);
  if (error_code) {
    error("json parse error: {}", error_code.message());
  }

  jv = jv.as_object();

  std::vector<std::string> install;
  auto arr = jv.at("install").as_array();
  for (const auto& item : arr) {
    install.emplace_back(item.as_string().c_str());
  }

  auto ports = jv.at("port").as_array();
  std::vector<Library> ret;

  for (const auto& item : ports) {
    ret.push_back(boost::json::value_to<Library>(item));
  }

  return {ret, install};
}

Library Program::get_from_name(const std::vector<Library>& libraries,
                               const std::string& name) {
  for (const auto& item : libraries) {
    if (boost::to_lower_copy(item.get_name()) == boost::to_lower_copy(name)) {
      return item;
    }
  }
  error("can not find this library: {}", name);
}

}  // namespace kpkg
