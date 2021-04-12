enable_testing()

if(KPKG_TEST_USE_PROXY)
  add_definitions(-DKPKG_TEST_USE_PROXY)
endif()

add_subdirectory(unit_test)

if(KPKG_BUILD_EXTRA_TEST)
  add_subdirectory(library)
endif()

include(Coverage)
include(Valgrind)
