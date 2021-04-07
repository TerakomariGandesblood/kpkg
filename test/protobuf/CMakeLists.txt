# https://cmake.org/cmake/help/latest/module/FindProtobuf.html
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Protobuf REQUIRED)

add_cxx_compiler_flag("-Wno-suggest-destructor-override")
add_cxx_compiler_flag("-Wno-padded")
add_cxx_compiler_flag("-Wno-global-constructors")
add_cxx_compiler_flag("-Wno-zero-as-null-pointer-constant")
add_cxx_compiler_flag("-Wno-sign-conversion")

add_executable(protobuf main.cpp addressbook.proto)
target_link_libraries(protobuf ${Protobuf_LIBRARIES})
protobuf_generate(TARGET protobuf)
