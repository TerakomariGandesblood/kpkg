cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(TBB REQUIRED)

add_executable(tbb main.cpp)
target_link_libraries(tbb TBB::tbb)

if(KPKG_USE_LIBCXX)
  target_link_libraries(tbb PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET tbb
  POST_BUILD
  COMMAND tbb)
