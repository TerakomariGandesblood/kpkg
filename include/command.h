#pragma once

#include <string>
#include <vector>

namespace kpkg {

void run_cmd(const std::string& cmd);

void run_cmds(const std::vector<std::string>& cmds, const std::string& cwd);

namespace detail {

inline const std::string export_gcc = "export CC=gcc-11 && export CXX=g++-11";

inline const std::string export_flag =
    R"(export CFLAGS="-fPIC" && export CXXFLAGS="-fPIC")";

std::string combine_cmd(const std::string& cmd1, const std::string& cmd2);

std::string calc_cmd(const std::vector<std::string>& cmds,
                     const std::string& cwd);

}  // namespace detail

}  // namespace kpkg
