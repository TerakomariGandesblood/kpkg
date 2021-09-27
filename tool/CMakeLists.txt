file(GLOB_RECURSE EXECUTABLE_SRC CONFIGURE_DEPENDS
     "${KPKG_SOURCE_DIR}/tool/*.cpp")

add_executable(${EXECUTABLE} ${EXECUTABLE_SRC})
target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARY} TBB::tbb spdlog::spdlog
                                            ${Boost_LIBRARIES} klib::klib)
