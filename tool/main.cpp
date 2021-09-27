#include <string>
#include <vector>

#include <klib/error.h>
#include <oneapi/tbb.h>
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

void build_libraries(const std::vector<kpkg::Library>& libraries,
                     const std::string& proxy) {
  tbb::concurrent_vector<kpkg::Library> copy(std::begin(libraries),
                                             std::end(libraries));

  tbb::task_group group;
  for (auto& item : copy) {
    group.run([&item, &proxy] {
      item.init(proxy);
      item.download(proxy);
      item.build();
    });
  }
  group.wait();
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
