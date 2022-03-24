#include "command.h"

#include <klib/exception.h>
#include <klib/log.h>
#include <klib/util.h>
#include <gsl/assert>

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
  // TODO temp
  bool gcc_12 = true;
  try {
    klib::exec("which gcc-12");
  } catch (const klib::RuntimeError& err) {
    gcc_12 = false;
  }

  if (gcc_12) {
    cmd = combine_command(cmd, "export CC=gcc-12 && export CXX=g++-12");
  } else {
    cmd = combine_command(cmd, "export CC=gcc-11 && export CXX=g++-11");
  }

  cmd = combine_command(
      cmd, R"(export CFLAGS="-pipe -march=haswell -O3 -g0 -DNDEBUG -fPIC")");
  cmd = combine_command(
      cmd, R"(export CXXFLAGS="-pipe -march=haswell -O3 -g0 -DNDEBUG -fPIC")");
  cmd = combine_command(
      cmd,
      R"(export LDFLAGS="-fuse-ld=bfd -static-libgcc -static-libstdc++ -s -L/usr/local/lib")");

  for (const auto& item : commands) {
    cmd = combine_command(cmd, item);
  }

  return cmd;
}

}  // namespace

void run_commands(const std::vector<std::string>& commands,
                  const std::string& dir) {
  auto cmd = calc_command(commands, dir);
  klib::info("Run commands: {}", cmd);
  klib::exec(cmd);
}

}  // namespace kpkg
