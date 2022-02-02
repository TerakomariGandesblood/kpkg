# ---------------------------------------------------------------------------------------
# CMake version
# ---------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.22)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# ---------------------------------------------------------------------------------------
# Start kpkg project
# ---------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Version)
kpkg_extract_version()

project(
  kpkg
  LANGUAGES ASM CXX
  VERSION ${KPKG_VERSION}
  HOMEPAGE_URL https://github.com/KaiserLancelot/kpkg)

message(STATUS "${PROJECT_NAME} version: ${KPKG_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose Debug, Release, RelWithDebInfo or MinSizeRel" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Set variables
# ---------------------------------------------------------------------------------------
set(KPKG_LIBRARY lib${PROJECT_NAME})
set(KPKG_EXECUTABLE ${PROJECT_NAME})
set(KPKG_TEST_EXECUTABLE ${KPKG_EXECUTABLE}-test)

set(KPKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KPKG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/command/file.html#glob-recurse
file(GLOB_RECURSE KPKG_LIBRARY_SRC CONFIGURE_DEPENDS
     "${KPKG_SOURCE_DIR}/src/*.cpp")

# ---------------------------------------------------------------------------------------
# Include CMake module
# ---------------------------------------------------------------------------------------
include(CTest)
include(GNUInstallDirs)

include(Options)
include(Check)
include(ClangTidy)
include(Format)
include(CompilerOptions)

# ---------------------------------------------------------------------------------------
# Find package
# ---------------------------------------------------------------------------------------
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(klib REQUIRED)
find_package(CLI11 REQUIRED)
find_package(semver REQUIRED)
find_package(simdjson REQUIRED)
find_package(Boost REQUIRED CONFIG)
find_package(gsl-lite REQUIRED)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_definitions(-Dgsl_CONFIG_CONTRACT_VIOLATION_THROWS)
endif()

if(NOT KPKG_SANITIZER)
  message(STATUS "Use mimalloc")
  set(MIMALLOC_OBJECT "/usr/local/lib/mimalloc.o")
endif()

# ---------------------------------------------------------------------------------------
# Build library json file
# ---------------------------------------------------------------------------------------
file(GLOB_RECURSE KPKG_LIBRARY_FILE CONFIGURE_DEPENDS
     "${KPKG_SOURCE_DIR}/library/*")
file(COPY ${KPKG_LIBRARY_FILE} DESTINATION ${KPKG_BINARY_DIR})

add_library(library_json STATIC "${KPKG_SOURCE_DIR}/src/library.s")
target_compile_options(library_json PRIVATE "-Wno-unused-command-line-argument")

# ---------------------------------------------------------------------------------------
# Build static library
# ---------------------------------------------------------------------------------------
add_library(${KPKG_LIBRARY} STATIC ${MIMALLOC_OBJECT} ${KPKG_LIBRARY_SRC})

target_include_directories(${KPKG_LIBRARY} PUBLIC "${KPKG_SOURCE_DIR}/include")
target_link_libraries(
  ${KPKG_LIBRARY}
  PRIVATE library_json
          fmt::fmt
          spdlog::spdlog
          semver::semver
          simdjson::simdjson
          ${Boost_LIBRARIES}
          klib::klib
          gsl::gsl-lite-v1)
set_target_properties(${KPKG_LIBRARY} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# ---------------------------------------------------------------------------------------
# Build executable
# ---------------------------------------------------------------------------------------
add_subdirectory(tool)

# ---------------------------------------------------------------------------------------
# Build test
# ---------------------------------------------------------------------------------------
if(BUILD_TESTING AND KPKG_BUILD_TEST)
  message(STATUS "Build test")
  add_subdirectory(test)
endif()

# ---------------------------------------------------------------------------------------
# Install target
# ---------------------------------------------------------------------------------------
include(Install)

# https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake
configure_file("${KPKG_SOURCE_DIR}/cmake/CMakeUninstall.cmake.in"
               "${KPKG_BINARY_DIR}/CMakeUninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P
                                    ${KPKG_BINARY_DIR}/CMakeUninstall.cmake)
