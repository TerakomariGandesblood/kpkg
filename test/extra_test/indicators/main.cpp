#include <chrono>
#include <cstdint>
#include <numeric>
#include <thread>
#include <vector>

#include <indicators/progress_bar.hpp>

// https://github.com/p-ranav/indicators
int main() {
  using namespace indicators;
  ProgressBar bar(option::BarWidth(50), option::Start("["), option::Fill("="),
                  option::Lead(">"), option::Remainder(" "), option::End("]"),
                  option::PostfixText("Init"),
                  option::ForegroundColor(Color::green),
                  option::FontStyles(std::vector<FontStyle>{FontStyle::bold}),
                  option::ShowPercentage(true));

  std::vector<std::int32_t> v(30);
  std::iota(std::begin(v), std::end(v), 0);
  bar.set_option(option::MaxProgress(30));

  for (auto i : v) {
    bar.set_option(option::PostfixText(std::to_string(i)));
    bar.tick();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(50ms);
  }
}
