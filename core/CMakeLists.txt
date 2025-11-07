add_library(core STATIC)

target_sources(core PRIVATE src/core.cpp)
target_include_directories(core PUBLIC include)
