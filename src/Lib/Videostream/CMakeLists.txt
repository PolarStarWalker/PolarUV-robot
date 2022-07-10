cmake_minimum_required(VERSION 3.16)

project(Videostream)

#compile .proto to c++ code
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS VideoDeviceMessage.proto)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(Videostream ${PROTO_SRCS} ${PROTO_HDRS}  Gstreamer.hpp Gstreamer.cpp)

target_link_libraries(Videostream ${CONAN_LIBS})
target_link_libraries(Videostream gstvideo4linux2)
target_link_libraries(Videostream gstcodecs-1.0 gstcodecparsers-1.0)