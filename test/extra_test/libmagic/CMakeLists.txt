cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.jpg
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(klib REQUIRED)
find_package(scope_guard REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(libmagic REQUIRED IMPORTED_TARGET libmagic)

add_executable(libmagic main.cpp)
target_link_libraries(libmagic PRIVATE klib::klib scope_guard::scope_guard
                                       PkgConfig::libmagic)

add_custom_command(
  TARGET libmagic
  POST_BUILD
  COMMAND libmagic)
