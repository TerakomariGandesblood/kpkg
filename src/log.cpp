#include "log.h"

#include <memory>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace kpkg {

void init_logger() {
  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto basic_file_sink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(".kpkg-log.txt");

  std::vector<spdlog::sink_ptr> sinks{stdout_sink, basic_file_sink};
  auto logger = std::make_shared<spdlog::logger>("kpkg", std::begin(sinks),
                                                 std::end(sinks));

  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);
}

}  // namespace kpkg
