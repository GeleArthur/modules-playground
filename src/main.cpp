#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

import EditorApplicationMod;

namespace
{
    EditorApplication app{};
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
    return app.Initialize();
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    return app.SDL_AppIterate(appstate);
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    return app.SDL_AppEvent(appstate, event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    app.SDL_AppQuit(appstate, result);
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
