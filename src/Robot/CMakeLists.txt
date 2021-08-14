project("Robot")

add_library(Robot Robot.hpp Robot.cpp)

add_subdirectory("DataProtocols" ${CMAKE_BINARY_DIR}/lib/DataProtocols)
add_subdirectory("Peripheral" ${CMAKE_BINARY_DIR}/lib/Peripheral)

target_link_libraries(Robot DataProtocols)
target_link_libraries(Robot Peripheral)

target_precompile_headers(Robot REUSE_FROM DataProtocols)
target_precompile_headers(Robot REUSE_FROM Peripheral)