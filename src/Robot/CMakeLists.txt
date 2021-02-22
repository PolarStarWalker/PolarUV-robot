project(Robot)

add_library(Robot Robot.hpp Robot.cpp)

add_subdirectory(Features)
add_subdirectory(Singleton)

target_link_libraries(Robot Features)
target_link_libraries(Robot Singleton)

target_precompile_headers(Robot REUSE_FROM Lib)

