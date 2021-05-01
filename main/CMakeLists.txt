# CMakeList.txt: проект CMake для main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.16)

file(COPY settings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (ROV "main.cpp" "main.hpp" "DataStruct.hpp")

add_subdirectory ("Matrix")
add_subdirectory ("Sensors")
add_subdirectory ("Services")
add_subdirectory ("DataProtocols")

target_precompile_headers(ROV REUSE_FROM Matrix)
target_precompile_headers(ROV REUSE_FROM Sensors)
target_precompile_headers(ROV REUSE_FROM DataProtocols)
target_precompile_headers(ROV REUSE_FROM Services)

target_link_libraries(ROV Matrix)
target_link_libraries(ROV Sensors)
target_link_libraries(ROV Services)
target_link_libraries(ROV DataProtocols)


