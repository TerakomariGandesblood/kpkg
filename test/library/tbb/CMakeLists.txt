cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

find_package(TBB REQUIRED)

add_executable(tbb main.cpp)
target_link_libraries(tbb TBB::tbb)
add_custom_command(
  TARGET tbb
  POST_BUILD
  COMMAND tbb)
