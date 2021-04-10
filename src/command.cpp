#include "command.h"

#include <cassert>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "error.h"

namespace kpkg {

void run_cmd(const std::string& cmd) {
  assert(!std::empty(cmd));

  spdlog::info("run cmd: {}", cmd);

  auto status = std::system(cmd.c_str());
  if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status)) {
    error("run cmd error");
  }
}

void run_cmds(const std::vector<std::string>& cmds, const std::string& cwd,
              Sanitize sanitize) {
  run_cmd(detail::calc_cmd(cmds, cwd, sanitize));
}

namespace detail {

std::string combine_cmd(const std::string& cmd1, const std::string& cmd2) {
  std::string cmd;

  if (!std::empty(cmd1) && !std::empty(cmd2)) {
    cmd = cmd1 + " && " + cmd2;
  } else if (std::empty(cmd1) && !std::empty(cmd2)) {
    cmd = cmd2;
  } else if (!std::empty(cmd1) && std::empty(cmd2)) {
    cmd = cmd1;
  }

  return cmd;
}

std::string deal_with_boost(const std::vector<std::string>& cmds,
                            const std::string& cwd, Sanitize sanitize) {
  std::string cmd = "cd " + cwd;

  if (sanitize == Sanitize::Memory) {
    cmd = combine_cmd(cmd, export_clang);
    cmd = combine_cmd(cmd, cmds.front());
    cmd = combine_cmd(cmd, cmds[2]);
  } else if (sanitize == Sanitize::Thread) {
    cmd = combine_cmd(cmd, export_clang);
    cmd = combine_cmd(cmd, cmds.front());
    cmd = combine_cmd(cmd, cmds[3]);
  } else {
    cmd = combine_cmd(cmd, export_gcc);
    cmd = combine_cmd(cmd, cmds.front());
    cmd = combine_cmd(cmd, cmds[1]);
  }

  return cmd;
}

std::string calc_cmd(const std::vector<std::string>& cmds,
                     const std::string& cwd, Sanitize sanitize) {
  assert(!std::empty(cmds) && !std::empty(cwd));

  if (cmds.front() == "./bootstrap.sh") {
    return deal_with_boost(cmds, cwd, sanitize);
  }

  std::string cmd = "cd " + cwd;

  if (sanitize == Sanitize::Memory) {
    cmd = combine_cmd(cmd, export_clang);
    cmd = combine_cmd(cmd, export_flag);
    cmd = combine_cmd(cmd, export_memory_flag);
  } else if (sanitize == Sanitize::Thread) {
    cmd = combine_cmd(cmd, export_clang);
    cmd = combine_cmd(cmd, export_flag);
    cmd = combine_cmd(cmd, export_thread_flag);
  } else {
    cmd = combine_cmd(cmd, export_gcc);
    cmd = combine_cmd(cmd, export_flag);
  }

  for (const auto& item : cmds) {
    cmd = combine_cmd(cmd, item);
  }

  return cmd;
}

}  // namespace detail

}  // namespace kpkg
