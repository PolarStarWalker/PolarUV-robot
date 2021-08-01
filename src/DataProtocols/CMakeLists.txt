cmake_minimum_required (VERSION 3.16)

project ("DataProtocols")

add_library(DataProtocols DataProtocols.hpp Socket.cpp SPI.cpp UART.cpp I2C.cpp CommandsProtocol.cpp SettingsProtocol.cpp)

target_precompile_headers(DataProtocols PUBLIC DataProtocols.hpp)
