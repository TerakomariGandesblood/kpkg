find_package(fmt REQUIRED)
add_executable(fmt main.cpp)
target_link_libraries(fmt fmt::fmt)
