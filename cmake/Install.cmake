include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Install executable
# ---------------------------------------------------------------------------------------
if(KPKG_BUILD_EXECUTABLE)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH
      "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR};$\{ORIGIN\}")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
       "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
  if(${isSystemDir} STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR};$\{ORIGIN\}")
  endif()

  install(TARGETS ${EXECUTABLE} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# ---------------------------------------------------------------------------------------
# Support creation of installable packages
# ---------------------------------------------------------------------------------------
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_INSTALL_CMAKE_PROJECTS ${KPKG_BINARY_DIR} ${LIBRARY} ALL .)

set(CPACK_PACKAGE_CONTACT "kaiser <KaiserLancelot123@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A template for C++ projects using CMake")
set(CPACK_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(CPACK_GENERATOR "TGZ;DEB")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "")
include(InstallRequiredSystemLibraries)

include(CPack)
