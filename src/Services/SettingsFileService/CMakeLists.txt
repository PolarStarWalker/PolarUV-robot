cmake_minimum_required (VERSION 3.16)

project ("SettingsFileService")

set(SOURCE_LIB SettingsFileService.cpp)

add_library(SettingsFileService STATIC ${SOURCE_LIB})

target_precompile_headers(SettingsFileService PUBLIC SettingsFileService.hpp)