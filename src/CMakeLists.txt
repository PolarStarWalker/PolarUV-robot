cmake_minimum_required (VERSION 3.16)

add_executable (robot "Startup.cpp" "Startup.hpp")

add_subdirectory ("Robot")
target_precompile_headers(robot REUSE_FROM Robot)
target_link_libraries(robot Robot)
