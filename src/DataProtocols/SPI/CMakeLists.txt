cmake_minimum_required (VERSION 3.16)

project ("SPI")

set(SOURCE_LIB SPI.cpp)

add_library(SPI STATIC ${SOURCE_LIB})

target_precompile_headers(SPI PUBLIC SPI.hpp)