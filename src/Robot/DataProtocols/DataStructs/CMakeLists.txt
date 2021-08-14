project ("DataStructs")

add_library(DataStructs SHARED CommandsStruct.cpp RobotSettingsStruct.cpp MotorsStruct.cpp)

target_precompile_headers(DataStructs PUBLIC DataStructs.hpp)