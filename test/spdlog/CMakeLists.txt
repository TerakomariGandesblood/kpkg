find_package(spdlog REQUIRED)
add_executable(spdlog main.cpp)
target_link_libraries(spdlog spdlog::spdlog)
