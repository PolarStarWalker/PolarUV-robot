project(Exceptions)

add_library(Exceptions Exceptions.hpp Handler.cpp)

target_precompile_headers(Exceptions PUBLIC Exceptions.hpp)