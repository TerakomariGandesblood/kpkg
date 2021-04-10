#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "library.h"

namespace kpkg {

std::tuple<std::vector<Library>, std::vector<Library>, Sanitize,
           std::vector<std::string>>
process_option(std::int32_t argc, char* argv[]);

}  // namespace kpkg