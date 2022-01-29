project ("DataStructs")

add_library(DataStructs STATIC CommandsStruct.cpp MotorsStruct.cpp VideoPipelineStruct.cpp)

target_precompile_headers(DataStructs PUBLIC DataStructs.hpp)