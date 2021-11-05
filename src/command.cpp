#include "command.h"

#include <cassert>

#include <klib/util.h>
#include <spdlog/spdlog.h>

namespace kpkg {

namespace {

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

}  // namespace

void run_commands(const std::vector<std::string>& commands,
                  const std::string& cwd) {
  auto cmd = detail::calc_command(commands, cwd);
  spdlog::info("Run commands: {}", cmd);
  klib::exec(cmd);
}

namespace detail {

std::string calc_command(const std::vector<std::string>& commands,
                         const std::string& cwd) {
  assert(!std::empty(commands) && !std::empty(cwd));

  std::string cmd = "cd " + cwd;

  // NOTE
  // Change the compiler version here
  cmd = combine_command(cmd, "export CC=gcc-11 && export CXX=g++-11");
  cmd = combine_command(
      cmd,
      R"(export CFLAGS="-g0 -O3 -fPIC -static-libgcc" && export CXXFLAGS="-g0 -O3 -fPIC -static-libgcc -static-libstdc++")");

  for (const auto& item : commands) {
    cmd = combine_command(cmd, item);
  }

  return cmd;
}

}  // namespace detail

}  // namespace kpkg
