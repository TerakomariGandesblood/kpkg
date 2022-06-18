#include "command.h"

#include <fmt/compile.h>
#include <fmt/format.h>
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

  bool use_ninja = true;
  try {
    klib::exec("which ninja");
  } catch (const klib::RuntimeError& err) {
    use_ninja = false;
  }

  if (use_ninja) {
    cmd = combine_command(cmd, "export CMAKE_GENERATOR=Ninja");
  }

  cmd = combine_command(cmd, "export CI=false");
  cmd = combine_command(cmd, "export CC=gcc-12 && export CXX=g++-12");

  // NOTE
  // Change compiler flags here
  // https://wiki.gentoo.org/wiki/GCC_optimization
  // https://www.phoronix.com/scan.php?page=article&item=gcc11-rocket-opts&num=4
  const std::string flags = "-pipe -maes -march=haswell -O3 -g0 -DNDEBUG -fPIC";
  const std::string ld_flags =
      "-fuse-ld=bfd -static-libgcc -static-libstdc++ -s";

  cmd = combine_command(
      cmd, fmt::format(FMT_COMPILE(R"(export CFLAGS="{}")"), flags));
  cmd = combine_command(
      cmd, fmt::format(FMT_COMPILE(R"(export CXXFLAGS="{}")"), flags));
  cmd = combine_command(
      cmd, fmt::format(FMT_COMPILE(R"(export LDFLAGS="{}")"), ld_flags));

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
