option(KPKG_BUILD_TEST "Build test" OFF)
option(KPKG_BUILD_EXTRA_TEST "Build extra test" OFF)

option(KPKG_TEST_USE_PROXY "Use proxy when running test" OFF)

option(KPKG_FORMAT "Format code using clang-format and cmake-format" OFF)
option(KPKG_CLANG_TIDY "Analyze code with clang-tidy" OFF)

option(KPKG_USE_LIBCXX "Use libc++" OFF)

include(CMakeDependentOption)
cmake_dependent_option(KPKG_BUILD_COVERAGE "Build with coverage information"
                       OFF "BUILD_TESTING;KPKG_BUILD_TEST" OFF)
cmake_dependent_option(KPKG_VALGRIND "Execute test with valgrind" OFF
                       "BUILD_TESTING;KPKG_BUILD_TEST" OFF)

set(KPKG_SANITIZER
    ""
    CACHE
      STRING
      "Build with a sanitizer. Options are: Address, Thread, Memory, Undefined and Address;Undefined"
)
