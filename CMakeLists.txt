cmake_minimum_required(VERSION 3.17)

# ---------------------------------------------------------------------------------------
# Start kpkg project
# ---------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(
  kpkg
  VERSION 0.1.0
  LANGUAGES CXX)

message(STATUS "Build ${PROJECT_NAME}: ${PROJECT_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose Release, Debug, MinSizeRel or RelWithDebInfo" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Set variables
# ---------------------------------------------------------------------------------------
set(LIBRARY lib${PROJECT_NAME})
set(EXECUTABLE ${PROJECT_NAME})
set(TESTS_EXECUTABLE catch)

set(KPKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KPKG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE LIBRARY_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE EXECUTABLE_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tool/*.cpp")

# ---------------------------------------------------------------------------------------
# Include
# ---------------------------------------------------------------------------------------
include(GNUInstallDirs)

include(Options)
include(Check)
include(ClangTidy)
include(CompilerOptions)
include(Format)
include(LTO)

add_subdirectory(test)

#find_package(Boost REQUIRED COMPONENTS json)
#find_package(CURL REQUIRED)
#find_package(fmt REQUIRED)
#find_package(spdlog REQUIRED)
#find_package(LibArchive REQUIRED)

# ---------------------------------------------------------------------------------------
# Build executable
# ---------------------------------------------------------------------------------------
#add_library(${LIBRARY} STATIC ${LIBRARY_SRC})
#target_include_directories(${LIBRARY}
#                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
#target_link_libraries(
#  ${LIBRARY} PRIVATE CURL::libcurl fmt::fmt spdlog::spdlog
#                     ${LibArchive_LIBRARIES} ${Boost_LIBRARIES})
#set_target_properties(${LIBRARY} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
#                                            DEBUG_POSTFIX d)
#
#add_executable(${EXECUTABLE} ${EXECUTABLE_SRC} include/command.h)
#target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARY})

# ---------------------------------------------------------------------------------------
# Install target
# ---------------------------------------------------------------------------------------
#if(KPKG_INSTALL)
#  message(STATUS "Generating install")
#  include(Install)
#endif()
