#include <unistd.h>
#include <wait.h>

#include <cstdint>
#include <cstdlib>

#include <klib/util.h>

#include "error.h"
#include "log.h"
#include "program.h"

int main(int argc, char* argv[]) {
  kpkg::init_logger();

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

    std::int32_t status = 0;

    while (waitpid(-1, &status, 0) > 0) {
      if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        kpkg::error("waitpid error");
      }
    }
    return EXIT_SUCCESS;
  }

  program.print_dependency();
  program.print_library_to_be_built();

  if (program.install_package()) {
    for (const auto& item : program.get_package_to_be_install()) {
      klib::util::execute_command(item);
    }
  }

  for (auto& item : program.get_dependency()) {
    item.init(program.use_proxy());
    item.download(program.use_proxy());
    item.build();
  }

#ifdef NDEBUG
  for (auto& item : program.get_library_to_be_built()) {
    auto pid{fork()};
    if (pid < 0) {
      kpkg::error("fork error");
    } else if (pid == 0) {
      item.init(program.use_proxy());
      item.download(program.use_proxy());
      item.build();
      return EXIT_SUCCESS;
    }
  }

  std::int32_t status{};

  while (waitpid(-1, &status, 0) > 0) {
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
      kpkg::error("waitpid error");
    }
  }
#else
  for (auto& item : program.get_library_to_be_built()) {
    item.init(program.use_proxy());
    item.download(program.use_proxy());
    item.build();
  }
#endif
}
