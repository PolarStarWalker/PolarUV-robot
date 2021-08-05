cmake_minimum_required (VERSION 3.16)

file(COPY settings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (robot "main.cpp" "main.hpp")

add_subdirectory ("Peripheral")

add_subdirectory ("DataProtocols")
add_subdirectory ("DataStructs")

target_precompile_headers(robot REUSE_FROM Peripheral)
target_precompile_headers(robot REUSE_FROM DataProtocols)
target_precompile_headers(robot REUSE_FROM DataStructs)

target_link_libraries(robot Peripheral)
target_link_libraries(robot DataProtocols)
target_link_libraries(robot DataStructs)


