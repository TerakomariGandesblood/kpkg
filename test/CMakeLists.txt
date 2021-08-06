enable_testing()

add_subdirectory(unit_test)

if(KPKG_BUILD_EXTRA_TEST)
  message(STATUS "Build extra test")
  add_subdirectory(library)
endif()

include(Coverage)
include(Valgrind)
