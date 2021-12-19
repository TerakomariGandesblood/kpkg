if(KPKG_TEST_USE_PROXY)
  add_definitions(-DKPKG_TEST_USE_PROXY)
endif()

file(GLOB_RECURSE KPKG_TEST_SRC CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(Catch2 REQUIRED)

add_executable(${KPKG_TEST_EXECUTABLE} ${MIMALLOC_OBJECT} ${KPKG_TEST_SRC})
target_link_libraries(${KPKG_TEST_EXECUTABLE} PRIVATE ${KPKG_LIBRARY}
                                                      Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(${KPKG_TEST_EXECUTABLE} REPORTER compact)
