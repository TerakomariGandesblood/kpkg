#include "command.h"

#include <wait.h>

#include <cassert>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "error.h"
#include "port.h"

namespace kpkg {

void run_cmd(const std::vector<std::string>& cmd, const std::string& cwd,
             Sanitize sanitize) {
  assert(!std::empty(cmd) && !std::empty(cwd));

  std::string temp;

  if (sanitize == Sanitize::Memory) {
    temp += export_clang + " && ";
    temp += export_memory_flag + " && ";
  } else if (sanitize == Sanitize::Thread) {
    temp += export_clang + " && ";
    temp += export_thread_flag + " && ";
  } else {
    temp += export_gcc + " && ";
  }

  temp += "cd " + cwd + " && ";

  for (const auto& item : cmd) {
    temp += (item + " && ");
  }
  temp += "sudo ldconfig";

  spdlog::info("run cmd: {}", temp);

  auto status = std::system(temp.c_str());
  if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status)) {
    kpkg::error("run cmd error");
  }
}

}  // namespace kpkg
