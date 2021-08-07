set(Boost_USE_STATIC_LIBS ON)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)

add_executable(compiler compiler.cpp)
target_link_libraries(compiler PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET compiler
  POST_BUILD
  COMMAND compiler)
