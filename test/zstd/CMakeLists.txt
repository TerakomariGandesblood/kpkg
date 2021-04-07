find_package(zstd REQUIRED)
add_executable(zstd main.cpp)
target_link_libraries(zstd zstd::libzstd_static)
