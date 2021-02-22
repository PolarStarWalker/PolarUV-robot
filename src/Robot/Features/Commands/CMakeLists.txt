project(Commands)

add_library(Commands Commands.cpp Commands.cpp CommandsCycle.cpp CommandsCycle.hpp)

target_link_libraries(Commands ${REQUIRED_LIBS_QUALIFIED})