# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project ("MS5837")

set(SOURCE_LIB MS5837.cpp)
add_library(MS5837 STATIC ${SOURCE_LIB})

target_precompile_headers(MS5837 PUBLIC MS5837.hpp)