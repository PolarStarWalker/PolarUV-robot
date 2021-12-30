project(Robot)

add_library(Robot Robot.hpp Robot.cpp)

add_subdirectory(Features ${CMAKE_BINARY_DIR}/lib/DataProtocols)
add_subdirectory(DataStructs ${CMAKE_BINARY_DIR}/lib/DataStructs)

target_link_libraries(Robot Features)
target_link_libraries(Robot DataStructs)

target_precompile_headers(Robot REUSE_FROM Features)
target_precompile_headers(Robot REUSE_FROM DataStructs)

