cmake_minimum_required (VERSION 3.16)

project ("DataProtocols")

add_subdirectory ("Socket")
add_subdirectory ("SPI")
add_subdirectory ("UART")

add_library(DataProtocols DataProtocols.hpp)

target_link_libraries(DataProtocols Socket)
target_link_libraries(DataProtocols SPI)
target_link_libraries(DataProtocols UART)

target_precompile_headers(DataProtocols REUSE_FROM Socket)
target_precompile_headers(DataProtocols REUSE_FROM UART)
