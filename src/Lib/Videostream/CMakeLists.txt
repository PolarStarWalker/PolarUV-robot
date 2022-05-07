cmake_minimum_required(VERSION 3.16)

project(Videostream)

find_package(OpenGL REQUIRED)

add_library(Videostream Videostream.hpp Videostream.cpp)

link_libraries(${CONAN_LIBS})

target_link_libraries(Videostream "/lib/aarch64-linux-gnu/libGL.so" "/lib/aarch64-linux-gnu/libEGL.so")
