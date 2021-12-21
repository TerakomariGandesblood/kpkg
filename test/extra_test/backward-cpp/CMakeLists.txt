cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(Backward REQUIRED)

add_executable(backward-cpp main.cpp)
target_link_libraries(backward-cpp PRIVATE Backward::Backward)
