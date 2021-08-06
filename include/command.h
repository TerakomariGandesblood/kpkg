#pragma once

#include <string>
#include <vector>

namespace kpkg {

void run_commands(const std::vector<std::string>& commands,
                  const std::string& cwd);

namespace detail {

inline const std::string export_gcc = "export CC=gcc-11 && export CXX=g++-11";

inline const std::string export_flag =
    R"(export CFLAGS="-fPIC" && export CXXFLAGS="-fPIC")";

std::string combine_command(const std::string& lhs, const std::string& rhs);

std::string calc_command(const std::vector<std::string>& commands,
                         const std::string& cwd);

}  // namespace detail

}  // namespace kpkg
