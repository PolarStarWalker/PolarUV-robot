project ("DataProtocols")

add_library(DataProtocols DataProtocols.hpp CommandsProtocol.cpp BaseProtocol.cpp RobotSettingsProtocol.cpp RobotVideoProtocol.cpp)

target_link_libraries(DataProtocols pthread)



