find_package(CURL REQUIRED)
add_executable(curl main.cpp)
target_link_libraries(curl CURL::libcurl)
