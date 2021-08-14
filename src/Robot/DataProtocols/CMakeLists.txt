project ("DataProtocols")

add_subdirectory(Matrix ${CMAKE_BINARY_DIR}/lib/Matrix)
add_subdirectory(DataTransmissions ${CMAKE_BINARY_DIR}/lib/DataTransmissions)
add_subdirectory(DataStructs ${CMAKE_BINARY_DIR}/lib/DataStructs)

add_library(DataProtocols DataProtocols.hpp CommandsProtocol.cpp BaseProtocol.cpp RobotSettingsProtocol.cpp)

target_link_libraries(DataProtocols Matrix)
target_link_libraries(DataProtocols DataTransmissions)
target_link_libraries(DataProtocols DataStructs)
target_link_libraries(DataProtocols pthread)

target_precompile_headers(DataProtocols REUSE_FROM DataTransmissions)
target_precompile_headers(DataProtocols REUSE_FROM Matrix)
target_precompile_headers(DataProtocols REUSE_FROM DataStructs)
