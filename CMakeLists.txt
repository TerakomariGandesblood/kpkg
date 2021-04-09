# https://github.com/gabime/spdlog/blob/v1.x/CMakeLists.txt
cmake_minimum_required(VERSION 3.17)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# ---------------------------------------------------------------------------------------
# Start kpkg project
# ---------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Version)
kpkg_extract_version()

project(
  kpkg
  VERSION ${KPKG_VERSION}
  LANGUAGES ASM CXX)

message(STATUS "${PROJECT_NAME} version: ${KPKG_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose Debug, Release, MinSizeRel or RelWithDebInfo" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Set variables
# ---------------------------------------------------------------------------------------
set(LIBRARY lib${PROJECT_NAME})
set(EXECUTABLE ${PROJECT_NAME})

set(KPKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KPKG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE PORT_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/port.s")
file(GLOB_RECURSE LIBRARY_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE EXECUTABLE_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tool/*.cpp")

# ---------------------------------------------------------------------------------------
# Include CMake module
# ---------------------------------------------------------------------------------------
include(CTest)
include(GNUInstallDirs)

include(Options)
include(Check)
include(ClangTidy)
include(CompilerOptions)
include(Format)

# ---------------------------------------------------------------------------------------
# Find package
# ---------------------------------------------------------------------------------------
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(Boost 1.75.0 CONFIG REQUIRED COMPONENTS json program_options)

# ---------------------------------------------------------------------------------------
# Build port
# ---------------------------------------------------------------------------------------
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/port/port.json"
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(port STATIC ${PORT_SRC})
target_compile_options(port PRIVATE "-Wno-unused-command-line-argument")

# ---------------------------------------------------------------------------------------
# Build static library
# ---------------------------------------------------------------------------------------
add_library(${LIBRARY} STATIC ${LIBRARY_SRC})
target_include_directories(${LIBRARY}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(
  ${LIBRARY}
  PRIVATE port
          CURL::libcurl
          fmt::fmt
          spdlog::spdlog
          ${Boost_LIBRARIES}
          ${LibArchive_LIBRARIES}
          ${ZLIB_LIBRARIES})

set_target_properties(
  ${LIBRARY}
  PROPERTIES DEBUG_POSTFIX d
             OUTPUT_NAME ${PROJECT_NAME}
             POSITION_INDEPENDENT_CODE ON
             INTERPROCEDURAL_OPTIMIZATION FALSE)

# ---------------------------------------------------------------------------------------
# Build executable
# ---------------------------------------------------------------------------------------
add_executable(${EXECUTABLE} ${EXECUTABLE_SRC})
target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARY})

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

configure_file("${KPKG_SOURCE_DIR}/cmake/CMakeUninstall.cmake.in"
               "${KPKG_BINARY_DIR}/CMakeUninstall.cmake" @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P
                                    ${KPKG_BINARY_DIR}/CMakeUninstall.cmake)
