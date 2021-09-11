cmake_minimum_required (VERSION 3.16)

project ("Peripheral")

add_library(Peripheral Peripheral.hpp LiftBag.cpp MS5837_I2C.cpp BNO055_I2C.cpp PeripheralHandler.cpp)
target_precompile_headers(Peripheral PUBLIC Peripheral.hpp)

