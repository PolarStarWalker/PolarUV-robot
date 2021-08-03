cmake_minimum_required (VERSION 3.16)

project ("DataProtocols")

add_subdirectory(DataTransmissions)

add_library(DataProtocols DataProtocols.hpp CommandsProtocol.cpp SettingsProtocol.cpp)

target_link_libraries(DataProtocols STATIC DataTransmissions)

target_precompile_headers(DataProtocols PUBLIC DataProtocols.hpp)
