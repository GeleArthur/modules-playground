add_library(game SHARED)

target_sources(game PRIVATE src/game.cpp)

target_include_directories(game PUBLIC include)