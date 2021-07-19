cmake_minimum_required (VERSION 3.16)

project ("UART")

set(SOURCE_LIB UART.cpp)

add_library(UART STATIC ${SOURCE_LIB})

target_precompile_headers(SPI PUBLIC UART.hpp)