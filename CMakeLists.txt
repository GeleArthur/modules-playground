cmake_minimum_required(VERSION 4.1.2)

set(CMAKE_CXX_MODULE_STD 1)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
project(modulesUI LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_executable(${PROJECT_NAME})

target_sources(modulesUI PRIVATE
        main.cpp

        PRIVATE
        FILE_SET cxx_modules
        TYPE CXX_MODULES FILES
        ApplicationModule.ixx
)

#set(OPTION "WEBGPU_BACKEND" CACHE STRING "WGPU" FORCE)
#set(OPTION "WEBGPU_BACKEND" CACHE STRING "DAWN" FORCE)

FetchContent_Declare(
        webgpu
        GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
        GIT_TAG 17dcd42a7683355e7a40ac4e97e77f36dff5b5ab
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(webgpu)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.24
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
        SDL3Webgpu
        GIT_REPOSITORY https://github.com/eliemichel/sdl3webgpu.git
        GIT_TAG 4fa9d70935e41c075f664b231ee10a64262a8ac7
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3Webgpu)


target_link_libraries(${PROJECT_NAME} PRIVATE webgpu SDL3::SDL3 sdl3webgpu)
target_copy_webgpu_binaries(${PROJECT_NAME})

if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            VERBATIM
    )
endif ()
