project(Lib)

add_subdirectory(Peripheral)
add_subdirectory(DataTransmissions)

add_library(Lib Lib.hpp Lib.cpp)

target_precompile_headers(Lib REUSE_FROM Peripheral)
target_precompile_headers(Lib REUSE_FROM DataTransmissions)

target_link_libraries(Lib Peripheral)
target_link_libraries(Lib DataTransmissions)
