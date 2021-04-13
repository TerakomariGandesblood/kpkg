cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

enable_testing()

find_package(Catch2 REQUIRED)
add_executable(catch2 main.cpp)
target_link_libraries(catch2 Catch2::Catch2WithMain)

if(KPKG_USE_LIBCXX)
  target_link_libraries(catch2 PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET catch2
  POST_BUILD
  COMMAND catch2)

include(Catch)
catch_discover_tests(catch2 REPORTER compact)
