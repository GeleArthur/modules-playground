#define SDL_MAIN_HANDLED
#include "sdl3webgpu.h"
#include <SDL3/SDL.h>

#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

import ApplicationModule;
import std;


int main(int argc, char* argv[])
{
    Application app{};
    app.Initialize();
    while (app.IsRunning())
    {
        app.MainLoop();
    }
    app.Terminate();

    // // Init WebGPU
    // WGPUInstance instance = wgpuCreateInstance(NULL);
    //
    // // Init SDL
    // SDL_Init(SDL_INIT_VIDEO);
    // SDL_Window *window = SDL_CreateWindow("Learn WebGPU", 640, 480, 0);
    //
    // // Here we create our WebGPU surface from the window!
    // WGPUSurface surface = SDL_GetWGPUSurface(instance, window);
    // std::printf("surface = %p", (void*)surface);
    //
    // // Wait for close
    // SDL_Event event;
    // int running = 1;
    // while (running)
    //     while (SDL_PollEvent(&event))
    //         if (event.type == SDL_EVENT_QUIT)
    //             running = 0;
    // // Terminate SDL
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return 0;
}
