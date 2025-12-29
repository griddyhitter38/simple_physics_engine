#ifndef SDL_APP_H
#define SDL_APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool init(SDL_Window **window, SDL_Renderer **renderer);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
float getDeltaTime(Uint32 *lastTime);

#endif
