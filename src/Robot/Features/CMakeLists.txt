project ("Features")
add_subdirectory(RobotSettings)
add_subdirectory(Video)
add_subdirectory(Sensors)

add_library(Features Features.hpp CommandsProtocol.cpp BaseProtocol.cpp RobotSettingsProtocol.cpp RobotVideoProtocol.cpp StartSettings.cpp)

target_link_libraries(Features pthread)
target_link_libraries(Features RobotSettings)
target_link_libraries(Features Video)
target_link_libraries(Features Sensors)



