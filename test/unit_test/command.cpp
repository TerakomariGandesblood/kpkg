#include <catch2/catch.hpp>

#include "command.h"

TEST_CASE("combine_cmd") {
  REQUIRE(kpkg::detail::combine_cmd("a", "b") == "a && b");
  REQUIRE(kpkg::detail::combine_cmd("", "b") == "b");
  REQUIRE(kpkg::detail::combine_cmd("a", "") == "a");
  REQUIRE(std::empty(kpkg::detail::combine_cmd("", "")));
}

TEST_CASE("calc_cmd") {
  REQUIRE(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd") ==
          "cd cwd && " + kpkg::detail::export_gcc + " && " +
              kpkg::detail::export_flag + " && configure && build && install");
}
