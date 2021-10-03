cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://cmake.org/cmake/help/latest/module/FindLibArchive.html
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(libarchive main.cpp)
target_link_libraries(libarchive PRIVATE ${LibArchive_LIBRARIES}
                                         ${ZLIB_LIBRARIES})

add_custom_command(
  TARGET libarchive
  POST_BUILD
  COMMAND libarchive libarchive libarchive.tar.gz
  COMMAND libarchive libarchive.tar.gz)
