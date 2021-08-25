cmake_minimum_required (VERSION 3.16)

project ("Peripheral")

add_library(Peripheral Peripheral.hpp LiftBag.cpp MS5837.cpp SensorsHandler.cpp BNO055/I2C_Copy.cpp BNO055.cpp BNO055/I2C_Copy.cpp)
target_precompile_headers(Peripheral PUBLIC Peripheral.hpp)

