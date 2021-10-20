# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project (Math)

set(SOURCE_LIB src/FloatMatrixClass.cpp src/IntMatrixClass.cpp src/FloatVectorClass.cpp src/IntVectorClass.cpp)

add_library(Math STATIC ${SOURCE_LIB})

target_precompile_headers(Math PUBLIC Math.hpp)