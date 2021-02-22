project (Features)

add_subdirectory(RobotSettings)
add_subdirectory(Video)
add_subdirectory(Sensors)
add_subdirectory(Commands)
add_subdirectory(StartSettings)

add_library(Features Features.hpp StartSettings.cpp)

target_precompile_headers(Features REUSE_FROM Lib)

target_link_libraries(Features pthread)
target_link_libraries(Features RobotSettings)
target_link_libraries(Features Video)
target_link_libraries(Features Sensors)
target_link_libraries(Features Commands)
target_link_libraries(Features StartSettings)


