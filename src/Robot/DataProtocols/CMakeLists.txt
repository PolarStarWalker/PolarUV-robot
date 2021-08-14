cmake_minimum_required (VERSION 3.16)

project ("DataProtocols")

add_subdirectory(Matrix)
add_subdirectory(DataTransmissions)

add_library(DataProtocols DataProtocols.hpp CommandsProtocol.cpp BaseProtocol.cpp RobotSettingsProtocol.cpp)

target_link_libraries(DataProtocols Matrix)
target_link_libraries(DataProtocols DataTransmissions)
#target_link_libraries(DataProtocols ../DataStructs)

target_precompile_headers(DataProtocols REUSE_FROM DataTransmissions)
target_precompile_headers(DataProtocols REUSE_FROM Matrix)
#target_precompile_headers(DataProtocols REUSE_FROM ../DataStructs)
