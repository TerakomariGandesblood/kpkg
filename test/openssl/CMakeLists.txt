# https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
find_package(OpenSSL REQUIRED)

add_executable(openssl main.cpp)
# OpenSSL::SSL OpenSSL::Crypto OPENSSL_LIBRARIES
target_link_libraries(openssl OpenSSL::Crypto)
