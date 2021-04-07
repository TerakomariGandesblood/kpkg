find_package(benchmark REQUIRED)

add_cxx_compiler_flag("-Wno-global-constructors")
add_executable(benchmark main.cpp)
target_link_libraries(benchmark benchmark::benchmark_main)
