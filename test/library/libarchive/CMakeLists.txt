cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# https://cmake.org/cmake/help/latest/module/FindLibArchive.html
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(libarchive main.cpp)
target_link_libraries(libarchive ${LibArchive_LIBRARIES} ${ZLIB_LIBRARIES})
add_custom_command(
  TARGET libarchive
  POST_BUILD
  COMMAND libarchive libarchive libarchive.tar.gz
  COMMAND libarchive libarchive.tar.gz)
