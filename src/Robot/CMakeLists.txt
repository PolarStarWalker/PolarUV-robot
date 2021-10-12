project("Robot")

add_library(Robot Robot.hpp Robot.cpp)

add_subdirectory("DataProtocols" ${CMAKE_BINARY_DIR}/lib/DataProtocols)
add_subdirectory("Peripheral" ${CMAKE_BINARY_DIR}/lib/Peripheral)
add_subdirectory("Server" ${CMAKE_BINARY_DIR}/lib/Server)
add_subdirectory("DataTransmissions" ${CMAKE_BINARY_DIR}/lib/DataTransmissions)
add_subdirectory("DataStructs" ${CMAKE_BINARY_DIR}/lib/DataStructs)
add_subdirectory("Matrix" ${CMAKE_BINARY_DIR}/lib/Matrix)

target_link_libraries(Robot DataProtocols)
target_link_libraries(Robot Peripheral)
target_link_libraries(Robot Server)
target_link_libraries(Robot DataTransmissions)
target_link_libraries(Robot DataStructs)
target_link_libraries(Robot Matrix)

target_precompile_headers(Robot REUSE_FROM DataProtocols)
target_precompile_headers(Robot REUSE_FROM Peripheral)
#target_precompile_headers(Robot REUSE_FROM Server)
target_precompile_headers(Robot REUSE_FROM DataStructs)
target_precompile_headers(Robot REUSE_FROM DataTransmissions)
target_precompile_headers(Robot REUSE_FROM Matrix)