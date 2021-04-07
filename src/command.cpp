#include "command.h"

#include <wait.h>

#include <cstdlib>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "error.h"

namespace kpkg {

void Command::run(const std::string& dir) const {
  run_command(configuration_, dir);
  run_command(build_, dir);
  run_command(install_, dir);
}

void Command::build_with_sanitize(Sanitize sanitize) {
  if (sanitize == Sanitize::None) {
    configuration_ =
        "export CC=gcc-10 && export CXX=g++-10 && " + configuration_;
    return;
  }

  configuration_ =
      "export CC=clang-12 && export CXX=clang++-12 && " + configuration_;

  if (sanitize == Sanitize::Memory) {
    add_flags(
        R"("-fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls")",
        R"("-fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++")");
  } else if (sanitize == Sanitize::Thread) {
    add_flags(R"("-fsanitize=thread")",
              R"("-fsanitize=thread -stdlib=libc++")");
  }
}

void Command::add_flags(std::string_view c_flags, std::string_view cxx_flags) {
  configuration_ = fmt::format("export CFLAGS={} && export CXXFLAGS={} && {}",
                               c_flags, cxx_flags, configuration_);
}

void run_command(const std::string& cmd, const std::string& dir) {
  if (std::empty(cmd)) {
    return;
  }

  std::string new_cmd;
  if (std::empty(dir)) {
    new_cmd = cmd;
  } else {
    new_cmd = "cd " + dir + " && " + cmd;
  }

  spdlog::info("run cmd: {}", new_cmd);

  auto status = std::system(new_cmd.c_str());
  if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status)) {
    kpkg::error("run cmd error");
  }
}

CMakeCommand::CMakeCommand(Sanitize sanitize) {
  std::string build_type =
      (sanitize == Sanitize::None ? "Release" : "RelWithDebInfo");

  configuration_ = "cmake -S . -B build -DCMAKE_BUILD_TYPE=" + build_type;

  build_ = R"delimiter(cmake --build build -j"$(nproc)" --config )delimiter" +
           build_type;
  install_ = "sudo cmake --build build --target install --config " + build_type;

  build_with_sanitize(sanitize);
}

void CMakeCommand::add_option(const std::string& key,
                              const std::string& value) {
  configuration_ += " -D" + key + "=" + value;
}

MakeCommand::MakeCommand(Sanitize sanitize) {
  configuration_ = "./configure";
  build_ = R"delimiter(make -j"$(nproc)")delimiter";
  install_ = "sudo make install";

  build_with_sanitize(sanitize);
}

void MakeCommand::add_option(const std::string& key, const std::string& value) {
  configuration_ += " --" + key + "=" + value;
}

B2Command::B2Command(Sanitize sanitize) {
  std::string toolset = sanitize == Sanitize::None ? "gcc-10" : "clang-12";

  configuration_ = "./bootstrap.sh";
  build_ = "sudo ./b2 toolset=" + toolset;

  build_with_sanitize(sanitize);

  build_ += " install";
}

void B2Command::add_option(const std::string& key, const std::string& value) {
  build_ += " " + key + "=" + value;
}

void B2Command::add_flags(std::string_view c_flags,
                          std::string_view cxx_flags) {
  fmt::format("{} cflags={} cxxflags={}", build_, c_flags, cxx_flags);
}

}  // namespace kpkg
