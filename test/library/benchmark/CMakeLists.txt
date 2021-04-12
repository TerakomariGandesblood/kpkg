cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

find_package(benchmark REQUIRED)

add_executable(benchmark main.cpp)
target_link_libraries(benchmark benchmark::benchmark_main)
add_custom_command(
  TARGET benchmark
  POST_BUILD
  COMMAND benchmark)
