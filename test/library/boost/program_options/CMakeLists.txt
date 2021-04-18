set(Boost_USE_STATIC_LIBS ON)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)

add_executable(compiler compiler.cpp)
target_link_libraries(compiler PRIVATE ${Boost_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(compiler PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET compiler
  POST_BUILD
  COMMAND compiler)

add_executable(pkg pkg.cpp)
target_link_libraries(pkg PRIVATE ${Boost_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(pkg PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

# add_custom_command( TARGET pkg POST_BUILD COMMAND pkg)
