#include "headers/sdl_app.h"

bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    return SDL_CreateWindowAndRenderer(1000, 800, SDL_WINDOW_SHOWN,
                                       window, renderer) == 0;
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

float getDeltaTime(Uint32 *lastTime)
{
    Uint32 now = SDL_GetTicks();
    float dt = (now - *lastTime) / 1000.0f;
    *lastTime = now;
    if (dt > 0.05f) dt = 0.05f;
    return dt;
}
