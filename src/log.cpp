#include "log.h"

#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace kpkg {

void init_logger() {
  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto logger = std::make_shared<spdlog::logger>("kpkg", stdout_sink);

  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);
}

}  // namespace kpkg
