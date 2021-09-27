#include <unistd.h>

#include <cstdlib>
#include <string>
#include <vector>

#include <klib/error.h>
#include <klib/util.h>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>

#include "library.h"
#include "log.h"
#include "program.h"

void print_libraries(const std::vector<kpkg::Library>& libraries) {
  if (std::empty(libraries)) {
    return;
  }

  std::vector<std::string> names;
  for (const auto& library : libraries) {
    names.push_back(library.get_name());
  }

  spdlog::info("The following libraries will be installed: {}",
               boost::join(names, " "));
}

void build_libraries(std::vector<kpkg::Library>& libraries,
                     const std::string& proxy) {
  for (auto& item : libraries) {
    auto pid = fork();
    if (pid < 0) {
      klib::error("Fork error");
    } else if (pid == 0) {
      item.init(proxy);
      item.download(proxy);
      item.build();
      std::exit(EXIT_SUCCESS);
    }
  }

  klib::wait_for_child_process();
}

int main(int argc, const char* argv[]) try {
  kpkg::init_logger();

  kpkg::Program program(argc, argv);

  print_libraries(program.dependencies());
  for (auto& item : program.dependencies()) {
    item.init(program.proxy());
    item.download(program.proxy());
    item.build();
  }

  print_libraries(program.libraries_to_be_built());
  build_libraries(program.libraries_to_be_built(), program.proxy());
} catch (const std::exception& err) {
  klib::error(err.what());
} catch (...) {
  klib::error("Unknown exception");
}
