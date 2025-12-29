#include "header/input_drag.h"
#include <SDL2/SDL.h>

bool pointInKirkle(int mx, int my, float cx, float cy, int r)
{
    float dx = mx - cx;
    float dy = my - cy;
    return dx*dx + dy*dy <= (float)(r*r);
}

bool pointInSquare(int mx, int my, const Square *s)
{
    return mx >= s->x - s->half && mx <= s->x + s->half &&
           my >= s->y - s->half && my <= s->y + s->half;
}

void handleEvents(bool *running,
                  DragTarget *dragTarget,
                  float *grabOffsetX, float *grabOffsetY,
                  float *cx, float *cy, float *cvx, float *cvy, int cr,
                  Square *square)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = false;
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;
            if (pointInSquare(mx, my, square)) {
                *dragTarget = DRAG_SQUARE;
                *grabOffsetX = mx - square->x;
                *grabOffsetY = my - square->y;
                square->vx = square->vy = 0.0f;
            } else if (pointInKirkle(mx, my, *cx, *cy, cr)) {
                *dragTarget = DRAG_KIRKLE;
                *grabOffsetX = mx - *cx;
                *grabOffsetY = my - *cy;
                *cvx = *cvy = 0.0f;
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
            *dragTarget = DRAG_NONE;
    }
}
