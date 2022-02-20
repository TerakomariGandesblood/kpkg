#include "command.h"

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
  cmd = combine_command(cmd, "export CC=gcc-11 && export CXX=g++-11");
  cmd = combine_command(
      cmd,
      R"(export CFLAGS="-pipe -march=haswell -mtune=haswell -fno-plt -fno-math-errno -fno-trapping-math -fno-semantic-interposition -fipa-pta -fgraphite-identity -O3 -g0 -DNDEBUG -fPIC -static-libgcc -s" && export CXXFLAGS="-pipe -march=haswell -mtune=haswell -fno-plt -fno-math-errno -fno-trapping-math -fno-semantic-interposition -fipa-pta -fgraphite-identity -O3 -g0 -DNDEBUG -fPIC -static-libgcc -static-libstdc++ -s")");
  cmd = combine_command(cmd, R"(export LDFLAGS="-L/usr/local/lib")");

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
