project(Lib)


add_subdirectory (Math)
target_precompile_headers(robot REUSE_FROM Robot)
target_link_libraries(robot Robot)
