cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

add_subdirectory(json)
add_subdirectory(asio)
add_subdirectory(pool)
add_subdirectory(string_algo)
add_subdirectory(core)
add_subdirectory(uuid)
