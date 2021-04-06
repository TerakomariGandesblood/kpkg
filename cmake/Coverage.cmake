if(KPKG_BUILD_COVERAGE)
  if(CMAKE_COMPILER_IS_GNUCXX)
    message(
      STATUS
        "Build tests with coverage information, use lcov to generate reports")

    get_filename_component(COMPILER_PATH ${CMAKE_CXX_COMPILER} PATH)
    string(REGEX MATCH "^[0-9]+" GCC_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    find_program(
      GCOV_EXECUTABLE
      NAMES gcov-${GCC_VERSION} gcov
      HINTS ${COMPILER_PATH})
    if(NOT GCOV_EXECUTABLE)
      message(FATAL_ERROR "Can not find gcov")
    endif()

    find_program(LCOV_EXECUTABLE lcov)
    if(NOT LCOV_EXECUTABLE)
      message(FATAL_ERROR "Can not find lcov")
    endif()

    find_program(GENHTML_EXECUTABLE genhtml)
    if(NOT GENHTML_EXECUTABLE)
      message(FATAL_ERROR "Can not find genhtml")
    endif()

    add_custom_target(
      coverage
      COMMAND ${LCOV_EXECUTABLE} -d . -z
      COMMAND ${TESTS_EXECUTABLE}
      COMMAND
        ${LCOV_EXECUTABLE} -d . --include '${KPKG_SOURCE_DIR}/src/*.cpp'
        --include '${KPKG_SOURCE_DIR}/include/*.h' --gcov-tool
        ${GCOV_EXECUTABLE} -c -o lcov.info --rc lcov_branch_coverage=1
      COMMAND ${GENHTML_EXECUTABLE} lcov.info -o coverage -s --title
              "${PROJECT_NAME}" --legend --demangle-cpp --branch-coverage
      WORKING_DIRECTORY ${KPKG_BINARY_DIR}
      DEPENDS ${TESTS_EXECUTABLE}
      COMMENT "Generating HTML report ${KPKG_BINARY_DIR}/coverage/index.html")
  else()
    message(
      STATUS
        "Build tests with coverage information, use llvm-cov to generate reports"
    )

    find_program(LLVM_PROFDATA_EXECUTABLE llvm-profdata)
    if(NOT LLVM_PROFDATA_EXECUTABLE)
      message(FATAL_ERROR "Can not find llvm-profdata")
    endif()

    find_program(LLVM_COV_EXECUTABLE llvm-cov)
    if(NOT LLVM_COV_EXECUTABLE)
      message(FATAL_ERROR "Can not find llvm-cov")
    endif()

    add_custom_target(
      coverage
      COMMAND ${TESTS_EXECUTABLE}
      COMMAND ${LLVM_PROFDATA_EXECUTABLE} merge -sparse -o
              ${TESTS_EXECUTABLE}.profdata default.profraw
      COMMAND
        ${LLVM_COV_EXECUTABLE} show ./test/${TESTS_EXECUTABLE}
        -instr-profile=${TESTS_EXECUTABLE}.profdata -format=html
        -show-branches=percent -show-line-counts-or-regions
        -ignore-filename-regex=${KPKG_SOURCE_DIR}/test/* -output-dir=coverage
      COMMAND
        ${LLVM_COV_EXECUTABLE} export ./test/${TESTS_EXECUTABLE}
        -instr-profile=${TESTS_EXECUTABLE}.profdata
        -ignore-filename-regex=${KPKG_SOURCE_DIR}/test/* -format=lcov >
        lcov.info
      WORKING_DIRECTORY ${KPKG_BINARY_DIR}
      DEPENDS ${TESTS_EXECUTABLE}
      COMMENT "Generating HTML report ${KPKG_BINARY_DIR}/coverage/index.html")
  endif()
endif()
