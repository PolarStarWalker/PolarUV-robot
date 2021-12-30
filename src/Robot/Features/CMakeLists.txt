project ("Features")

add_library(Features Features.hpp CommandsProtocol.cpp BaseProtocol.cpp RobotSettingsProtocol.cpp RobotVideoProtocol.cpp StartSettings.cpp)

target_link_libraries(Features pthread)



