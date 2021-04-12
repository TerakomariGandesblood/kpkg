cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

find_package(zstd REQUIRED)
add_executable(zstd main.cpp)
target_link_libraries(zstd zstd::libzstd_static)
add_custom_command(
  TARGET zstd
  POST_BUILD
  COMMAND zstd zstd
  COMMAND zstd zstd.zst)
