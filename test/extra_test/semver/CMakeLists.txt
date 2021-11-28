cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(semver REQUIRED)

add_executable(semver main.cpp)
target_link_libraries(semver semver::semver)

add_custom_command(
  TARGET semver
  POST_BUILD
  COMMAND semver)
