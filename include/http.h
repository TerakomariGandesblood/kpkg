#pragma once

#include <string>

#include <klib/http.h>

namespace kpkg {

klib::Response http_get(const std::string &url, const std::string &proxy);

}  // namespace kpkg
