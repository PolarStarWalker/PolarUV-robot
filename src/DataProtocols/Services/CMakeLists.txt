project ("Services")

add_library(Services Services.hpp SettingsFileService.cpp PeripheralHandlerService.cpp)

target_precompile_headers(Services PUBLIC Services.hpp)
