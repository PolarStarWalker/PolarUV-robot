
cmake_minimum_required (VERSION 3.16)

project ("Socket")

set(SOURCE_LIB Socket.cpp)

add_library(Socket STATIC ${SOURCE_LIB})

target_precompile_headers(Socket PUBLIC Socket.hpp)