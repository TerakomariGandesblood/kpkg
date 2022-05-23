# ---------------------------------------------------------------------------------------
# Install executable
# ---------------------------------------------------------------------------------------
include(GNUInstallDirs)

install(
  TARGETS ${KPKG_EXECUTABLE}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ---------------------------------------------------------------------------------------
# Support creation of installable packages
# ---------------------------------------------------------------------------------------
# https://cmake.org/cmake/help/latest/cpack_gen/deb.html
# https://cmake.org/cmake/help/latest/module/CPack.html
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_INSTALL_CMAKE_PROJECTS ${KPKG_BINARY_DIR} ${PROJECT_NAME} ALL .)

# https://cmake.org/cmake/help/latest/cpack_gen/deb.html
set(CPACK_PACKAGE_CONTACT "Kaiser <kaiserlancelot123@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ Library Manager")
set(CPACK_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# https://cmake.org/cmake/help/latest/cpack_gen/archive.html
set(CPACK_GENERATOR "TXZ;DEB")
# https://cmake.org/cmake/help/latest/cpack_gen/deb.html
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
# https://cmake.org/cmake/help/latest/module/CPack.html#variable:CPACK_THREADS
set(CPACK_THREADS 0)
# https://cmake.org/cmake/help/latest/cpack_gen/deb.html#variable:CPACK_DEBIAN_PACKAGE_SHLIBDEPS
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
