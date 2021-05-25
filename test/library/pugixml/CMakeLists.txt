cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.xml
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(pugixml REQUIRED)

add_executable(pugixml-test main.cpp)
target_link_libraries(pugixml-test PRIVATE pugixml::pugixml)

add_custom_command(
  TARGET pugixml-test
  POST_BUILD
  COMMAND pugixml-test)
