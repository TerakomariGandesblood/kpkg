#pragma once

#include <string>
#include <vector>

namespace kpkg {

void run_commands(const std::vector<std::string>& commands,
                  const std::string& cwd);

namespace detail {

std::string calc_command(const std::vector<std::string>& commands,
                         const std::string& cwd);

}  // namespace detail

}  // namespace kpkg
