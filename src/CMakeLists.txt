cmake_minimum_required (VERSION 3.16)

file(COPY settings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (robot "main.cpp" "main.hpp")

add_subdirectory ("Robot")
target_precompile_headers(robot REUSE_FROM Robot)
target_link_libraries(robot Robot)


