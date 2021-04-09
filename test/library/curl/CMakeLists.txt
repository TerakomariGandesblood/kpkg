cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

find_package(CURL CONFIG REQUIRED)
add_executable(curl main.cpp)
target_link_libraries(curl CURL::libcurl)
add_custom_command(
  TARGET curl
  POST_BUILD
  COMMAND curl -p www.baidu.com)
