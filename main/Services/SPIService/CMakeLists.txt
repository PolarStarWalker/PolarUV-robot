cmake_minimum_required (VERSION 3.16)

project ("SPIService")

set(SOURCE_LIB SPIService.cpp)

add_library(SPIService STATIC ${SOURCE_LIB})

target_precompile_headers(SPIService PUBLIC SPIService.hpp)