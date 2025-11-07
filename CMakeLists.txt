cmake_minimum_required(VERSION 4.1.2)


# Sadly emscripten doesn't supports this yet!?!?!
#set(CMAKE_CXX_MODULE_STD 1)
#set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
project(modulesUI LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE Debug)
include(FetchContent)

#add_executable(${PROJECT_NAME})
#
#target_sources(modulesUI PRIVATE
#        src/main.cpp
#
#        PRIVATE
#        FILE_SET cxx_modules
#        TYPE CXX_MODULES FILES
#        src/EditorApplication.ixx
#)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore *)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.24
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.4-docking
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_sources(imgui PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp

        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_link_libraries(imgui PRIVATE SDL3::SDL3)


#target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui)

#if (WIN32)
#    add_custom_command(
#            TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#            VERBATIM
#    )
#endif ()

if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE -sASYNCIFY)
endif ()

add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(editor)
add_subdirectory(glue)

