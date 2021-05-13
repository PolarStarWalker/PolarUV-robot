cmake_minimum_required (VERSION 3.16)

project ("I2C")

set(SOURCE_LIB I2C.cpp)

add_library(I2C STATIC ${SOURCE_LIB})

target_precompile_headers(I2C PUBLIC I2C.hpp)