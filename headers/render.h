#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "physics.h"

void renderSquare(SDL_Renderer *renderer, const Square *s);
void drawFilledKirkle(SDL_Renderer *renderer, int cx, int cy, int radius);
void renderr(SDL_Renderer *renderer, float x, float y, int radius, const Square *sq);

#endif
