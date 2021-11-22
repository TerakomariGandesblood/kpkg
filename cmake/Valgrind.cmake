if(KPKG_VALGRIND)
  message(STATUS "Execute test with valgrind")

  find_program(VALGRIND_EXECUTABLE valgrind)

  if(NOT VALGRIND_EXECUTABLE)
    message(FATAL_ERROR "Can not find valgrind")
  endif()

  add_test(
    NAME ${KPKG_TEST_EXECUTABLE}-valgrind
    COMMAND
      ${VALGRIND_EXECUTABLE} --leak-check=full --show-leak-kinds=all
      --leak-resolution=med --track-origins=yes --vgdb=no --tool=memcheck
      --gen-suppressions=all --error-exitcode=1 ./${KPKG_TEST_EXECUTABLE}
    WORKING_DIRECTORY ${KPKG_BINARY_DIR}/test/unit_test)

  set(VALGRIND_CMD
      ${VALGRIND_EXECUTABLE}
      --leak-check=full
      --show-leak-kinds=all
      --leak-resolution=med
      --track-origins=yes
      --vgdb=no
      --tool=memcheck
      --gen-suppressions=all
      --error-exitcode=1
      ${KPKG_BINARY_DIR}/tool/${KPKG_EXECUTABLE})

  if(KPKG_TEST_USE_PROXY)
    add_test(NAME kpkg-install-valgrind COMMAND ${VALGRIND_CMD} install spdlog
                                                -p)
  else()
    add_test(NAME kpkg-install-valgrind COMMAND ${VALGRIND_CMD} install spdlog)
  endif()
endif()
