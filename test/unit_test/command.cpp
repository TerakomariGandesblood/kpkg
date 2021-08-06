#include <catch2/catch.hpp>

#include "command.h"

TEST_CASE("combine_command") {
  REQUIRE(kpkg::detail::combine_command("a", "b") == "a && b");
  REQUIRE(kpkg::detail::combine_command("", "b") == "b");
  REQUIRE(kpkg::detail::combine_command("a", "") == "a");
  REQUIRE(std::empty(kpkg::detail::combine_command("", "")));
}

TEST_CASE("calc_command") {
  REQUIRE(
      kpkg::detail::calc_command({"configure", "build", "install"}, "cwd") ==
      "cd cwd && " + kpkg::detail::export_gcc + " && " +
          kpkg::detail::export_flag + " && configure && build && install");
}
