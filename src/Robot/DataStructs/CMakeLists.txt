project ("DataStructs")

add_library(DataStructs CommandsStruct.cpp RobotSettingsStruct.cpp MotorsStruct.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

target_link_libraries(DataStructs ${REQUIRED_LIBS_QUALIFIED})

target_precompile_headers(DataStructs PUBLIC DataStructs.hpp)