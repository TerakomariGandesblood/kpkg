#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// https://github.com/gabime/spdlog
int main() {
  spdlog::init_thread_pool(8192, 1);

  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto basic_file_sink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt");

  std::vector<spdlog::sink_ptr> sinks{stdout_sink, basic_file_sink};
  auto logger = std::make_shared<spdlog::async_logger>(
      "async_logger", std::begin(sinks), std::end(sinks),
      spdlog::thread_pool());
  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);

  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("Welcome to spdlog!");
  spdlog::debug("Some debug message");
}
