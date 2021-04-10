#include <catch2/catch.hpp>

#include "command.h"

TEST_CASE("combine_cmd") {
  REQUIRE(kpkg::detail::combine_cmd("a", "b") == "a && b");
  REQUIRE(kpkg::detail::combine_cmd("", "b") == "b");
  REQUIRE(kpkg::detail::combine_cmd("a", "") == "a");
  REQUIRE(std::empty(kpkg::detail::combine_cmd("", "")));
}

TEST_CASE("deal_with_boost") {
  REQUIRE(kpkg::detail::deal_with_boost(
              {"./bootstrap.sh", "none", "memory", "thread"}, "boost/???",
              kpkg::Sanitize::None) == "cd boost/??? && " +
                                           kpkg::detail::export_gcc +
                                           " && ./bootstrap.sh && none");

  REQUIRE(kpkg::detail::deal_with_boost(
              {"./bootstrap.sh", "none", "memory", "thread"}, "boost",
              kpkg::Sanitize::Memory) == "cd boost && " +
                                             kpkg::detail::export_clang +
                                             " && ./bootstrap.sh && memory");

  REQUIRE(kpkg::detail::deal_with_boost(
              {"./bootstrap.sh", "none", "memory", "thread"}, "boost",
              kpkg::Sanitize::Thread) == "cd boost && " +
                                             kpkg::detail::export_clang +
                                             " && ./bootstrap.sh && thread");
}

TEST_CASE("calc_cmd") {
  REQUIRE(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                 kpkg::Sanitize::None) ==
          "cd cwd && " + kpkg::detail::export_gcc + " && " +
              kpkg::detail::export_flag + " && configure && build && install");

  REQUIRE(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                 kpkg::Sanitize::Memory) ==
          "cd cwd && " + kpkg::detail::export_clang + " && " +
              kpkg::detail::export_memory_flag +
              " && configure && build && install");

  REQUIRE(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                 kpkg::Sanitize::Thread) ==
          "cd cwd && " + kpkg::detail::export_clang + " && " +
              kpkg::detail::export_thread_flag +
              " && configure && build && install");
}
