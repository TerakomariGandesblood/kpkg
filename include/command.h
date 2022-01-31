#pragma once

#include <string>
#include <vector>

namespace kpkg {

void run_commands(const std::vector<std::string>& commands,
                  const std::string& dir);

}  // namespace kpkg
