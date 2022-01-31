file(GLOB_RECURSE KPKG_EXECUTABLE_SRC CONFIGURE_DEPENDS
     "${KPKG_SOURCE_DIR}/tool/*.cpp")

add_executable(${KPKG_EXECUTABLE} ${MIMALLOC_OBJECT} ${KPKG_EXECUTABLE_SRC})
target_link_libraries(
  ${KPKG_EXECUTABLE} PRIVATE ${KPKG_LIBRARY} spdlog::spdlog CLI11::CLI11
                             ${Boost_LIBRARIES} klib::klib)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  find_package(Backward REQUIRED)
  target_link_libraries(${KPKG_EXECUTABLE} PRIVATE Backward::Backward)
endif()
