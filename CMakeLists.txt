# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

set(CMAKE_CXX_FLAGS "-fcoroutines")

# build options
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    add_definitions(-DDEBUG)
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

project("robot")

file(COPY ./Conan/conanfile.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ./Conan/conan.cmake DESTINATION ${CMAKE_BINARY_DIR})

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE starwalker
        REMOTE conancenter
        SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Включите подпроекты.
add_subdirectory("src")
