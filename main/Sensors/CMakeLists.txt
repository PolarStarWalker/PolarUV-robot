# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project ("Sensors")

add_subdirectory ("MS5837")

add_library(Sensors Sensors.hpp)
target_link_libraries(Sensors MS5837)
target_precompile_headers(Sensors REUSE_FROM MS5837)

