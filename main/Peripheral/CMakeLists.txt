cmake_minimum_required (VERSION 3.16)

project ("Peripheral")

add_subdirectory ("MS5837")

add_library(Peripheral Peripheral.hpp LiftBag/LiftBag.cpp LiftBag/LiftBag.hpp)
target_link_libraries(Peripheral MS5837)
target_precompile_headers(Peripheral REUSE_FROM MS5837)

