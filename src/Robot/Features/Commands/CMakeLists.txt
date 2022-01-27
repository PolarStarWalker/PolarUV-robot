project(Commands)

add_library(Commands Commands.cpp Commands.cpp)

target_link_libraries(Commands ${REQUIRED_LIBS_QUALIFIED})