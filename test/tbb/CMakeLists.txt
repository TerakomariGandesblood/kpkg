find_package(TBB REQUIRED)

add_executable(tbb main.cpp)
target_link_libraries(tbb TBB::tbb)
