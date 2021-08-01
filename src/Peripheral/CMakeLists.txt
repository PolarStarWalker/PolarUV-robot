cmake_minimum_required (VERSION 3.16)

project ("Peripheral")

add_subdirectory ("MS5837")

add_library(Peripheral Peripheral.hpp LiftBag.cpp MS5837.cpp)
target_precompile_headers(Peripheral PUBLIC Peripheral.hpp)

