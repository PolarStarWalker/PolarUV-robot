cmake_minimum_required (VERSION 3.16)

project (TcpSession)

add_library(TcpSession TcpSession.hpp TcpSession.cpp)
target_link_libraries(DataTransmissions ${CONAN_LIBS})

target_precompile_headers(TcpSession PUBLIC TcpSession.hpp)