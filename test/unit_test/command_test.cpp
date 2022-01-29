#include <catch2/catch.hpp>

#include "command.h"

TEST_CASE("calc_command", "[command]") {
  REQUIRE(
      kpkg::detail::calc_command({"configure", "build", "install"}, "cwd") ==
      R"(cd cwd && export CI=false && export CC=gcc-11 && export CXX=g++-11 && export CFLAGS="-fuse-ld=gold -march=x86-64 -mtune=generic -g0 -DNDEBUG -O3 -fPIC -static-libgcc" && export CXXFLAGS="-fuse-ld=gold -march=x86-64 -mtune=generic -g0 -DNDEBUG -O3 -fPIC -static-libgcc -static-libstdc++" && configure && build && install)");
}
