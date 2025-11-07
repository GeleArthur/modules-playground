#include <SDL3/SDL.h>

int main() {
    SDL_Init(0);

    auto* window = SDL_CreateWindow(nullptr, 640, 640, 0);

    SDL_Event event;
    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
    }

    SDL_Quit();
}