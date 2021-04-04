
cmake_minimum_required (VERSION 3.16)

project ("Services")

add_subdirectory ("SensorsHandlerService")
add_subdirectory ("SettingsFileService")
#add_subdirectory ("SettingsFileService")
add_subdirectory ("SPIService")


add_library(Services Services.hpp)

target_link_libraries(Services SettingsFileService)
target_link_libraries(Services SensorsHandlerService)
target_link_libraries(Services SPIService)

target_precompile_headers(Services REUSE_FROM SettingsFileService)
target_precompile_headers(Services REUSE_FROM SensorsHandlerService)
target_precompile_headers(Services REUSE_FROM SPIService)
