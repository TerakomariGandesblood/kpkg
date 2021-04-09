#include <unistd.h>
#include <wait.h>

#include <cstdint>

#include <fmt/format.h>

#include "command.h"
#include "error.h"
#include "option.h"
#include "port.h"

int main(int argc, char* argv[]) {
  auto [pre, lib, san] = kpkg::process_option(argc, argv);

  for (const auto& item : kpkg::install) {
    kpkg::run_cmd(item);
  }

  fmt::print("pre: \n");
  for (const auto& item : pre) {
    item.print();
  }
  fmt::print("lib: \n");
  for (const auto& item : lib) {
    item.print();
  }

  for (auto& item : pre) {
    item.init();
    item.download();
    item.build(san);
  }

  for (auto& item : lib) {
    auto pid{fork()};
    if (pid < 0) {
      kpkg::error("fork error");
    } else if (pid == 0) {
      item.init();
      item.download();
      item.build(san);
      return EXIT_SUCCESS;
    }
  }

  std::int32_t status{};

  while (waitpid(-1, &status, 0) > 0) {
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
      kpkg::error("Error");
    }
  }
}
