cmake_minimum_required(VERSION 3.16)

project(Videostream)

add_library(Videostream Gstreamer.hpp Gstreamer.cpp)

target_link_libraries(Videostream ${CONAN_LIBS})
target_link_libraries(Videostream gstvideo4linux2)
target_link_libraries(Videostream gstcodecs-1.0 gstcodecparsers-1.0)