cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
find_package(OpenSSL REQUIRED)

add_executable(openssl main.cpp)
# OpenSSL::SSL OpenSSL::Crypto OPENSSL_LIBRARIES
target_link_libraries(openssl OpenSSL::Crypto)
add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND openssl)
