enable_testing()

find_package(Catch2 REQUIRED)
add_executable(catch2 main.cpp)
target_link_libraries(catch2 Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(catch2 REPORTER compact)
