#include <unistd.h>
#include <wait.h>

#include <cstdint>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "command.h"
#include "error.h"
#include "program.h"

int main(int argc, char* argv[]) {
  kpkg::Program program(argc, argv);

  if (program.get_type() == kpkg::Program::Type::List) {
    for (auto& item : program.get_libraries()) {
      auto pid = fork();
      if (pid < 0) {
        kpkg::error("fork error");
      } else if (pid == 0) {
        item.init(program.use_proxy());
        item.print();
        return EXIT_SUCCESS;
      }
    }

    std::int32_t status{};

    while (waitpid(-1, &status, 0) > 0) {
      if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        kpkg::error("Error");
      }
    }
    return EXIT_SUCCESS;
  }

  spdlog::set_level(spdlog::level::debug);

  program.print_dependency();
  program.print_library_to_be_built();

  if (program.install_package()) {
    for (const auto& item : program.get_package_to_be_install()) {
      kpkg::run_cmd(item);
    }
  }

  for (auto& item : program.get_dependency()) {
    item.init(program.use_proxy());
    item.download(program.use_proxy());
    item.build(program.get_sanitize());
  }

#ifdef NDEBUG
  for (auto& item : program.get_library_to_be_built()) {
    auto pid{fork()};
    if (pid < 0) {
      kpkg::error("fork error");
    } else if (pid == 0) {
      item.init(program.use_proxy());
      item.download(program.use_proxy());
      item.build(program.get_sanitize());
      return EXIT_SUCCESS;
    }
  }

  std::int32_t status{};

  while (waitpid(-1, &status, 0) > 0) {
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
      kpkg::error("Error");
    }
  }
#else
  for (auto& item : program.get_library_to_be_built()) {
    item.init(program.use_proxy());
    item.download(program.use_proxy());
    item.build(program.get_sanitize());
  }
#endif
}
