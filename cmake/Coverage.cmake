if(KPKG_BUILD_COVERAGE)
  if(CMAKE_COMPILER_IS_GNUCXX)
    message(
      STATUS "Build test with coverage information, use lcov to generate report"
    )

    find_program(LCOV_EXECUTABLE lcov)
    if(NOT LCOV_EXECUTABLE)
      message(FATAL_ERROR "Can not find lcov")
    endif()

    find_program(GENHTML_EXECUTABLE genhtml)
    if(NOT GENHTML_EXECUTABLE)
      message(FATAL_ERROR "Can not find genhtml")
    endif()

    # https://github.com/linux-test-project/lcov
    add_custom_target(
      coverage
      COMMAND ${LCOV_EXECUTABLE} -d ${KPKG_BINARY_DIR} -z
      COMMAND ${KPKG_TEST_EXECUTABLE}
      COMMAND
        ${LCOV_EXECUTABLE} -d ${KPKG_BINARY_DIR} --include
        '${KPKG_SOURCE_DIR}/src/*.cpp' --include
        '${KPKG_SOURCE_DIR}/include/*.h' -c -o lcov.info --rc
        lcov_branch_coverage=1
      COMMAND ${GENHTML_EXECUTABLE} lcov.info -o coverage -s --title
              "${PROJECT_NAME}" --legend --demangle-cpp --branch-coverage
      COMMAND rm -rf ${KPKG_BINARY_DIR}/coverage
      COMMAND mv lcov.info ${KPKG_BINARY_DIR}/lcov.info
      COMMAND mv coverage ${KPKG_BINARY_DIR}/coverage
      WORKING_DIRECTORY ${KPKG_BINARY_DIR}/test/unit_test
      DEPENDS ${KPKG_TEST_EXECUTABLE}
      COMMENT "Generate HTML report: ${KPKG_BINARY_DIR}/coverage/index.html")
  else()
    message(
      STATUS
        "Build test with coverage information, use llvm-cov to generate report")

    find_program(LLVM_PROFDATA_EXECUTABLE llvm-profdata)
    if(NOT LLVM_PROFDATA_EXECUTABLE)
      message(FATAL_ERROR "Can not find llvm-profdata")
    endif()

    find_program(LLVM_COV_EXECUTABLE llvm-cov)
    if(NOT LLVM_COV_EXECUTABLE)
      message(FATAL_ERROR "Can not find llvm-cov")
    endif()

    # https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
    # https://llvm.org/docs/CommandGuide/llvm-cov.html
    add_custom_target(
      coverage
      COMMAND ${KPKG_TEST_EXECUTABLE}
      COMMAND ${LLVM_PROFDATA_EXECUTABLE} merge -sparse -o
              ${KPKG_TEST_EXECUTABLE}.profdata default.profraw
      COMMAND
        ${LLVM_COV_EXECUTABLE} show ./${KPKG_TEST_EXECUTABLE}
        -instr-profile=${KPKG_TEST_EXECUTABLE}.profdata -show-branches=percent
        -show-line-counts-or-regions
        -ignore-filename-regex=${KPKG_SOURCE_DIR}/test/* -format=html
        -output-dir=coverage
      COMMAND
        ${LLVM_COV_EXECUTABLE} export ./${KPKG_TEST_EXECUTABLE}
        -instr-profile=${KPKG_TEST_EXECUTABLE}.profdata -format=lcov
        -ignore-filename-regex=${KPKG_SOURCE_DIR}/test/* > lcov.info
      COMMAND rm -rf ${KPKG_BINARY_DIR}/coverage
      COMMAND mv lcov.info ${KPKG_BINARY_DIR}/lcov.info
      COMMAND mv coverage ${KPKG_BINARY_DIR}/coverage
      WORKING_DIRECTORY ${KPKG_BINARY_DIR}/test/unit_test
      DEPENDS ${KPKG_TEST_EXECUTABLE}
      COMMENT "Generate HTML report: ${KPKG_BINARY_DIR}/coverage/index.html")
  endif()
endif()
