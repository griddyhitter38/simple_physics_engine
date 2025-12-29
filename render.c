#include "render.h"
#include <math.h>

void renderSquare(SDL_Renderer *renderer, const Square *s)
{
    SDL_SetRenderDrawColor(renderer, s->R, s->G, s->B, s->A);
    SDL_Rect r = { (int)(s->x - s->half), (int)(s->y - s->half),
                   s->half * 2, s->half * 2 };
    SDL_RenderFillRect(renderer, &r);
}

void drawFilledKirkle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; dy++) {
        int y = cy + dy;
        int dx = (int)sqrt(radius*radius - dy*dy);
        SDL_RenderDrawLine(renderer, cx - dx, y, cx + dx, y);
    }
}

void renderr(SDL_Renderer *renderer, float x, float y, int radius, const Square *sq)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawFilledKirkle(renderer, (int)x, (int)y, radius);
    renderSquare(renderer, sq);

    SDL_RenderPresent(renderer);
}
