#pragma once

#include <string>
#include <vector>

extern char port[];
extern int port_size;

namespace kpkg {

inline std::vector<std::string> install;

inline std::string export_gcc;

inline std::string export_clang;

inline std::string export_flag;

inline std::string export_memory_flag;

inline std::string export_thread_flag;

}  // namespace kpkg
