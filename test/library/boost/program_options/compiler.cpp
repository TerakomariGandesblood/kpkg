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

// https://www.boost.org/doc/libs/1_76_0/doc/html/program_options.html
class ProgramOptions {
 public:
  ProgramOptions(std::int32_t argc, const char* argv[]) {
    boost::program_options::options_description generic("Generic options");
    generic.add_options()("version,v", "print version string")(
        "help,h", "produce help message");

    boost::program_options::options_description config("Configuration");
    config.add_options()(
        "optimization,O",
        boost::program_options::value<std::int32_t>(&opt_)->default_value(0),
        "optimization level")(
        "include-path,I",
        boost::program_options::value<std::vector<std::string>>(&include_path_),
        "include path");

    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()(
        "input-file",
        boost::program_options::value<std::vector<std::string>>(&input_file_));

    boost::program_options::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    boost::program_options::options_description visible("Allowed options");
    visible.add(generic).add(config);

    boost::program_options::positional_options_description p;
    p.add("input-file", -1);

    boost::program_options::variables_map vm;
    store(boost::program_options::command_line_parser(argc, argv)
              .options(cmdline_options)
              .positional(p)
              .run(),
          vm);
    notify(vm);

    if (vm.contains("help")) {
      std::cout << visible << '\n';
      // visible.
      std::exit(EXIT_SUCCESS);
    }

    if (vm.contains("version")) {
      std::cout << argv[0] << " version: 0.1.0\n";
      std::exit(EXIT_SUCCESS);
    }
  }

  [[nodiscard]] int32_t get_opt() const { return opt_; }

  [[nodiscard]] const std::vector<std::string>& get_include_path() const {
    return include_path_;
  }

  [[nodiscard]] const std::vector<std::string>& get_input_file() const {
    return input_file_;
  }

 private:
  std::int32_t opt_ = 0;
  std::vector<std::string> include_path_;
  std::vector<std::string> input_file_;
};

int main() try {
  const char* argv[] = {"0", "-I/usr", "-O3", "b", "-I/usr/local", "a"};
  std::int32_t argc = std::size(argv);

  ProgramOptions options(argc, argv);

  EXPECT(options.get_opt() == 3);
  EXPECT((options.get_include_path() ==
          std::vector<std::string>{"/usr", "/usr/local"}));
  EXPECT((options.get_input_file() == std::vector<std::string>{"b", "a"}));
} catch (const std::exception& err) {
  std::cerr << err.what() << "\n";
  return EXIT_FAILURE;
}
