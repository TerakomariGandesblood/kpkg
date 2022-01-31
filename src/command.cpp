#include "command.h"

#include <klib/util.h>
#include <spdlog/spdlog.h>
#include <gsl/gsl-lite.hpp>

namespace kpkg {

namespace {

std::string combine_command(const std::string& lhs, const std::string& rhs) {
  Expects(!std::empty(lhs) && !std::empty(rhs));
  return lhs + " && " + rhs;
}

std::string calc_command(const std::vector<std::string>& commands,
                         const std::string& dir) {
  Expects(!std::empty(commands) && !std::empty(dir));

  std::string cmd = "cd " + dir;

  cmd = combine_command(cmd, "export CI=false");
  // NOTE
  // Change the compiler version here
  cmd = combine_command(cmd, "export CC=gcc-11 && export CXX=g++-11");
  cmd = combine_command(
      cmd,
      R"(export CFLAGS="-march=x86-64 -mtune=generic -g0 -DNDEBUG -O3 -fPIC -static-libgcc" && export CXXFLAGS="-march=x86-64 -mtune=generic -g0 -DNDEBUG -O3 -fPIC -static-libgcc -static-libstdc++")");

  for (const auto& item : commands) {
    cmd = combine_command(cmd, item);
  }

  return cmd;
}

}  // namespace

void run_commands(const std::vector<std::string>& commands,
                  const std::string& dir) {
  auto cmd = calc_command(commands, dir);
  spdlog::info("Run commands: {}", cmd);
  klib::exec(cmd);
}

}  // namespace kpkg
