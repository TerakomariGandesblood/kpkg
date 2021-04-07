# https://cmake.org/cmake/help/latest/module/FindZLIB.html
find_package(ZLIB REQUIRED)
add_executable(zlib main.cpp)
target_link_libraries(zlib ${ZLIB_LIBRARIES})
