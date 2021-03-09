# CMakeList.txt: проект CMake для main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

file(COPY settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory ("Matrix")
add_subdirectory ("Socket")
# Добавьте источник в исполняемый файл этого проекта.
add_executable (ROV "main.cpp" "main.h")
target_link_libraries(ROV Matrix)
target_link_libraries(ROV Socket)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
