cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/zlib-v1.2.11.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(fmt REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)
add_definitions(-DOPENSSL_NO_DEPRECATED)

add_executable(openssl main.cpp)
target_link_libraries(openssl PRIVATE PkgConfig::openssl fmt::fmt)

add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND openssl)
