project("Robot")

add_library(Robot Robot.hpp Robot.cpp)

add_subdirectory("DataStructs")
add_subdirectory("DataProtocols")
add_subdirectory("Peripheral")

target_link_libraries(Robot DataStructs)
target_link_libraries(Robot DataProtocols)
target_link_libraries(Robot Peripheral)

target_precompile_headers(Robot REUSE_FROM DataStructs)
target_precompile_headers(Robot REUSE_FROM DataProtocols)
target_precompile_headers(Robot REUSE_FROM Peripheral)