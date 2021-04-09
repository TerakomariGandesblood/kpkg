#pragma once

#include <string>
#include <vector>

#include "library.h"

namespace kpkg {

void run_cmd(const std::string& cmd);

void run_cmd(const std::vector<std::string>& cmd, const std::string& cwd,
             Sanitize sanitize);

}  // namespace kpkg
