enable_testing()

if(KPKG_TEST_USE_PROXY)
  add_definitions(-DKPKG_TEST_USE_PROXY)
endif()

# add_subdirectory(library)
add_subdirectory(unit_test)
