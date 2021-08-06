#include "command.h"

#include <cassert>

#include <klib/util.h>
#include <spdlog/spdlog.h>

namespace kpkg {

void run_commands(const std::vector<std::string>& commands,
                  const std::string& cwd) {
  auto cmd = detail::calc_command(commands, cwd);
  spdlog::info("Run command: {}");
  klib::util::execute_command(cmd);
}

namespace detail {

std::string combine_command(const std::string& lhs, const std::string& rhs) {
  std::string cmd;

  if (!std::empty(lhs) && !std::empty(rhs)) {
    cmd = lhs + " && " + rhs;
  } else if (std::empty(lhs) && !std::empty(rhs)) {
    cmd = rhs;
  } else if (!std::empty(lhs) && std::empty(rhs)) {
    cmd = lhs;
  } else {
    assert(false);
  }

  return cmd;
}

std::string calc_command(const std::vector<std::string>& commands,
                         const std::string& cwd) {
  assert(!std::empty(commands) && !std::empty(cwd));

  std::string cmd = "cd " + cwd;

  cmd = combine_command(cmd, export_gcc);
  cmd = combine_command(cmd, export_flag);

  for (const auto& item : commands) {
    cmd = combine_command(cmd, item);
  }

  return cmd;
}

}  // namespace detail

}  // namespace kpkg
