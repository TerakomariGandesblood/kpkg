#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "error.h"

// https://gist.github.com/randomphrase/10801888
class ProgramOptions {
 public:
  enum class Type { Install, List };

  ProgramOptions(std::int32_t argc, const char* argv[]) {
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
      std::cout << argv[0] << " version: 0.1.0\n";
      std::exit(EXIT_SUCCESS);
    }

    if (command == "install") {
      type_ = Type::Install;

      boost::program_options::options_description install_config(
          "Install configuration");
      install_config.add_options()("memory,m", "Use memory")("thread,t",
                                                             "Use thread");

      boost::program_options::options_description install_hidden(
          "Hidden options");
      install_hidden.add_options()(
          "install-libraries",
          boost::program_options::value<std::vector<std::string>>(
              &install_libraries_));

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
    }

    else if (command == "list") {
      type_ = Type::List;
    } else {
      throw boost::program_options::invalid_option_value(command);
    }
  }

  [[nodiscard]] Type get_type() const { return type_; }

  [[nodiscard]] const std::vector<std::string>& get_install_libraries() const {
    return install_libraries_;
  }

 private:
  Type type_;
  std::vector<std::string> install_libraries_;
};

int main() {
  {
    const char* argv[] = {"0", "install", "a", "b", "d", "c"};
    std::int32_t argc = std::size(argv);

    ProgramOptions options(argc, argv);
    EXPECT(options.get_type() == ProgramOptions::Type::Install);
    EXPECT((options.get_install_libraries() ==
            std::vector<std::string>{"a", "b", "d", "c"}));
  }

  {
    const char* argv[] = {"0", "list"};
    std::int32_t argc = std::size(argv);

    ProgramOptions options(argc, argv);
    EXPECT(options.get_type() == ProgramOptions::Type::List);
  }
}
