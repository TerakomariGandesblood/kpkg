#include <unistd.h>
#include <wait.h>

#include <cstdint>

#include "error.h"
#include "option.h"

int main(int argc, char* argv[]) {
  auto [pre, lib, san] = kpkg::process_option(argc, argv);

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
