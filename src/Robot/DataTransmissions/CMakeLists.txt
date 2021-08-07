cmake_minimum_required (VERSION 3.16)

project ("DataTransmissions")

add_library(DataTransmissions DataTransmissions.hpp Socket.cpp SPI.cpp UART.cpp I2C.cpp)
