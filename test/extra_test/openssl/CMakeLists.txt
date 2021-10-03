cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/zlib-v1.2.11.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(fmt REQUIRED)
# https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
find_package(OpenSSL REQUIRED)
add_definitions(-DOPENSSL_NO_DEPRECATED)

add_executable(openssl main.cpp)
target_link_libraries(openssl PRIVATE ${OPENSSL_LIBRARIES} fmt::fmt)

add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND openssl)
