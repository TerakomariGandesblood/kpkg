#pragma once

#include <string_view>
#include <utility>

#include <spdlog/spdlog.h>

namespace kpkg {

void init_logger();

// TODO
// Use this function
// https://github.com/gabime/spdlog/issues/2023

// template <typename... Args>
// void log(std::string_view fmt, Args &&...args) {
//   spdlog::info(fmt, std::forward<Args>(args)...);
// }

}  // namespace kpkg
