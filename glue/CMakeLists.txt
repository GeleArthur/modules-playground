add_executable(glue)

target_sources(glue PRIVATE main.cpp)
target_link_libraries(glue PRIVATE SDL3::SDL3)

target_link_libraries(glue PRIVATE core)
target_link_libraries(glue PRIVATE editor)
target_link_libraries(glue PRIVATE game)
