# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project ("LiftBag")

set(SOURCE_LIB LiftBag.cpp)
add_library(LiftBag STATIC ${SOURCE_LIB})

target_precompile_headers(LiftBag PUBLIC LiftBag.hpp)