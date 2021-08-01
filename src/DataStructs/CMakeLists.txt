project ("DataStructs")

add_library(DataStructs CommandsStruct.cpp SettingsStruct.cpp MotorsStruct.cpp)

target_precompile_headers(DataStructs PUBLIC DataStructs.hpp)