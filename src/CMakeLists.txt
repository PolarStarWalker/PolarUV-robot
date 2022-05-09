cmake_minimum_required (VERSION 3.16)

include_directories(Lib)

add_subdirectory (Lib)
add_subdirectory (Robot)

add_executable (robot "Startup.cpp" "Startup.hpp")

target_precompile_headers(robot REUSE_FROM Robot)

target_link_libraries(robot Robot)
target_link_libraries(robot Lib)
target_link_libraries(robot ${CONAN_LIBS})

