cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

enable_testing()

find_package(Catch2 REQUIRED)
add_executable(catch2 main.cpp)
target_link_libraries(catch2 Catch2::Catch2WithMain)
add_custom_command(
  TARGET catch2
  POST_BUILD
  COMMAND catch2)

include(Catch)
catch_discover_tests(catch2 REPORTER compact)
