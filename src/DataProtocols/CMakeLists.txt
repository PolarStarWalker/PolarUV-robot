cmake_minimum_required (VERSION 3.16)

project ("DataProtocols")

add_subdirectory(DataTransmissions)
add_subdirectory(Matrix)
add_subdirectory(Services)

add_library(DataProtocols DataProtocols.hpp CommandsProtocol.cpp SettingsProtocol.cpp)

target_link_libraries(DataProtocols DataTransmissions)
target_link_libraries(DataProtocols Matrix)
target_link_libraries(DataProtocols Services)

target_precompile_headers(DataProtocols PUBLIC DataProtocols.hpp)
