# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project ("SensorsHandlerService")

set(SOURCE_LIB SensorsHandlerService.cpp)

add_library(SensorsHandlerService STATIC ${SOURCE_LIB})

target_precompile_headers(SensorsHandlerService PUBLIC SensorsHandlerService.hpp)