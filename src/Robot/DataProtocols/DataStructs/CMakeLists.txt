project ("DataStructs")

add_library(DataStructs SHARED CommandsStruct.cpp RobotSettingsStruct.cpp MotorsStruct.cpp VideoPipelineStruct.cpp)

target_precompile_headers(DataStructs PUBLIC DataStructs.hpp)