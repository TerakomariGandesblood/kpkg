cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
find_package(OpenSSL REQUIRED)

add_executable(openssl main.cpp)
# OpenSSL::SSL OpenSSL::Crypto OPENSSL_LIBRARIES
target_link_libraries(openssl OpenSSL::Crypto)

if(KPKG_USE_LIBCXX)
  target_link_libraries(openssl PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND openssl)
