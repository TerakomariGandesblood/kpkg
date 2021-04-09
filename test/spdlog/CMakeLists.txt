cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

find_package(spdlog REQUIRED)
add_executable(spdlog main.cpp)
target_link_libraries(spdlog spdlog::spdlog)
add_custom_command(
  TARGET spdlog
  POST_BUILD
  COMMAND spdlog)
