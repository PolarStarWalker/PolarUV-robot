# CMakeList.txt: проект CMake для main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.16)

file(COPY settings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (robot "main.cpp" "main.hpp" "DataStruct.hpp")

add_subdirectory ("Matrix")
add_subdirectory ("Peripheral")
add_subdirectory ("Services")
add_subdirectory ("DataProtocols")

target_precompile_headers(robot REUSE_FROM Matrix)
target_precompile_headers(robot REUSE_FROM Peripheral)
target_precompile_headers(robot REUSE_FROM DataProtocols)
target_precompile_headers(robot REUSE_FROM Services)

target_link_libraries(robot Matrix)
target_link_libraries(robot Peripheral)
target_link_libraries(robot Services)
target_link_libraries(robot DataProtocols)


