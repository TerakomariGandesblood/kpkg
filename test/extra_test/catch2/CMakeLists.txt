cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

enable_testing()

# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
find_package(Catch2 REQUIRED)
add_definitions(-DCATCH_CONFIG_ENABLE_BENCHMARKING)

add_executable(catch2 main.cpp)
target_link_libraries(catch2 PRIVATE Catch2::Catch2WithMain)

add_custom_command(
  TARGET catch2
  POST_BUILD
  COMMAND catch2)

include(Catch)
catch_discover_tests(catch2 REPORTER compact)
