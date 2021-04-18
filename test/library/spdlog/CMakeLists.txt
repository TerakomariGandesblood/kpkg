cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(spdlog REQUIRED)

add_executable(spdlog main.cpp)
target_link_libraries(spdlog PRIVATE spdlog::spdlog)

if(KPKG_USE_LIBCXX)
  target_link_libraries(spdlog PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET spdlog
  POST_BUILD
  COMMAND spdlog)
