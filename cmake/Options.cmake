option(KPKG_BUILD_TEST "Build test" OFF)
option(KPKG_BUILD_EXTRA_TEST "Build extra test" OFF)
option(KPKG_TEST_USE_PROXY "Use proxy when running test" OFF)

option(KPKG_FORMAT "Format code using clang-format and cmake-format" OFF)
option(KPKG_SANITIZER "Build with AddressSanitizer and UndefinedSanitizer" OFF)

include(CMakeDependentOption)
cmake_dependent_option(
  KPKG_BUILD_COVERAGE "Build test with coverage information" OFF
  "BUILD_TESTING;KPKG_BUILD_TEST" OFF)
