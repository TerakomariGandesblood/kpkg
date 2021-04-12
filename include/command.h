#pragma once

#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

void run_cmd(const std::string& cmd);

void run_cmds(const std::vector<std::string>& cmds, const std::string& cwd,
              Sanitize sanitize);

namespace detail {

inline const std::string export_gcc = "export CC=gcc-10 && export CXX=g++-10";

inline const std::string export_clang =
    "export CC=clang-12 && export CXX=clang++-12";

inline const std::string export_flag =
    R"(export CFLAGS="-fPIC" && export CXXFLAGS="-fPIC")";

inline const std::string export_memory_flag =
    R"(export CFLAGS="-fPIC -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls" && export CXXFLAGS="-fPIC -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++ -Wno-unused-command-line-argument -Wl,/usr/local/lib/libc++abi.a -Wl,/usr/local/lib/libc++.a")";

inline const std::string export_thread_flag =
    R"(export CFLAGS="-fPIC -fsanitize=thread" && export CXXFLAGS="-fPIC -fsanitize=thread -stdlib=libc++ -Wno-unused-command-line-argument -Wl,/usr/local/lib/libc++abi.a -Wl,/usr/local/lib/libc++.a")";

std::string combine_cmd(const std::string& cmd1, const std::string& cmd2);

std::string deal_with_boost(const std::vector<std::string>& cmds,
                            const std::string& cwd, kpkg::Sanitize sanitize);

std::string calc_cmd(const std::vector<std::string>& cmds,
                     const std::string& cwd, Sanitize sanitize);

}  // namespace detail

}  // namespace kpkg
