# CMakeList.txt: проект CMake для main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

add_subdirectory ("Matrix")
add_subdirectory ("Socket")
# Добавьте источник в исполняемый файл этого проекта.
add_executable (main "main.cpp" "main.h")
target_link_libraries(main Matrix)
target_link_libraries(main Socket)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
