#pragma once

#include <string>

namespace kpkg {

std::string get_page(const std::string& url, bool use_proxy);

void get_file(const std::string& url, const std::string& file_name,
              bool use_proxy);

}  // namespace kpkg
