# ---------------------------------------------------------------------------------------
# Install executable
# ---------------------------------------------------------------------------------------
include(GNUInstallDirs)

# https://stackoverflow.com/questions/30398238/cmake-rpath-not-working-could-not-find-shared-object-file
set_target_properties(
  ${KPKG_EXECUTABLE}
  PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
             INSTALL_RPATH_USE_LINK_PATH TRUE)

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
set(CPACK_PACKAGE_CONTACT "Kaiser <KaiserLancelot123@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ Library Manager")
set(CPACK_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# https://cmake.org/cmake/help/latest/cpack_gen/archive.html
# https://cmake.org/cmake/help/latest/cpack_gen/deb.html
set(CPACK_GENERATOR "TZST;DEB")
# FIXME set(CPACK_DEBIAN_COMPRESSION_TYPE "zstd")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
