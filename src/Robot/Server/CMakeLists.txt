project (Server)

add_library(Server STATIC Server.hpp Core.cpp)

target_precompile_headers(Server PUBLIC Server.hpp)