# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.16)

project ("Matrix")

set(SOURCE_LIB src/FloatMatrixClass.cpp  src/IntMatrixClass.cpp)

add_library(Matrix STATIC ${SOURCE_LIB})

target_precompile_headers(Matrix PUBLIC Matrix.hpp)