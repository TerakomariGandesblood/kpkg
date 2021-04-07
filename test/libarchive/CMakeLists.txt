# https://cmake.org/cmake/help/latest/module/FindLibArchive.html
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(libarchive main.cpp)
target_link_libraries(libarchive ${LibArchive_LIBRARIES} ${ZLIB_LIBRARIES})
