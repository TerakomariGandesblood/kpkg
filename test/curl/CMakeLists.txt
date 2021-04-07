find_package(CURL CONFIG REQUIRED)
add_executable(curl main.cpp)
target_link_libraries(curl CURL::libcurl)
