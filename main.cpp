#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_MAIN_HANDLED
#include "sdl3webgpu.h"
#include <SDL3/SDL.h>


#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

import ApplicationModule;

namespace
{
    Application app{};
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
    app.Initialize();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    app.MainLoop();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    return app.EventHandle(event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    app.Terminate();
}

// int main(int argc, char* argv[])
// {
//     Application app{};
//     app.Initialize();
//     while (app.IsRunning())
//     {
//         app.MainLoop();
//     }
//     app.Terminate();
//
//     // // Init WebGPU
//     // WGPUInstance instance = wgpuCreateInstance(NULL);
//     //
//     // // Init SDL
//     // SDL_Init(SDL_INIT_VIDEO);
//     // SDL_Window *window = SDL_CreateWindow("Learn WebGPU", 640, 480, 0);
//     //
//     // // Here we create our WebGPU surface from the window!
//     // WGPUSurface surface = SDL_GetWGPUSurface(instance, window);
//     // std::printf("surface = %p", (void*)surface);
//     //
//     // // Wait for close
//     // SDL_Event event;
//     // int running = 1;
//     // while (running)
//     //     while (SDL_PollEvent(&event))
//     //         if (event.type == SDL_EVENT_QUIT)
//     //             running = 0;
//     // // Terminate SDL
//     // SDL_DestroyWindow(window);
//     // SDL_Quit();
//
//     return 0;
// }
