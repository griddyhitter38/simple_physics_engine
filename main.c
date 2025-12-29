#include <SDL2/SDL.h>
#include <stdbool.h>

#include "sdl_app.h"
#include "physics.h"
#include "render.h"
#include "input_drag.h"

int main(void)
{
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init(&window, &renderer)) return 1;

    Square sq = {
        .x = 300.0f, .y = 300.0f,
        .vx = 0.0f, .vy = 0.0f,
        .ax = 0.0f, .ay = 0.0f,
        .half = 50,
        .R = 255, .G = 255, .B = 255, .A = 255
    };

    int radius = 70;

    float x = WIDTH / 2.0f;
    float y = HEIGHT / 2.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float ax = 0.0f;
    float ay = 0.0f;

    DragTarget drag = DRAG_NONE;
    float grabOffX = 0.0f, grabOffY = 0.0f;

    int prevMx = 0, prevMy = 0;
    SDL_GetMouseState(&prevMx, &prevMy);

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        handleEvents(&running, &drag,
                     &grabOffX, &grabOffY,
                     &x, &y, &vx, &vy, radius,
                     &sq);

        float dt = getDeltaTime(&lastTime);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        float mdx = (float)(mx - prevMx);
        float mdy = (float)(my - prevMy);
        prevMx = mx;
        prevMy = my;

        float throwVx = (dt > 0.0f) ? mdx / dt : 0.0f;
        float throwVy = (dt > 0.0f) ? mdy / dt : 0.0f;

        if (drag == DRAG_KIRKLE) {
            x = (float)mx - grabOffX;
            y = (float)my - grabOffY;
            vx = throwVx;
            vy = throwVy;
        } else {
            updatePhysics(&x, &y, &vx, &vy, &ax, &ay, radius, dt);
        }

        if (drag == DRAG_SQUARE) {
            sq.x = (float)mx - grabOffX;
            sq.y = (float)my - grabOffY;
            sq.vx = throwVx;
            sq.vy = throwVy;
        } else {
            updateSquare(&sq, dt);
        }

        resolveKirkleSquareCollision(&x, &y, &vx, &vy, radius, &sq);
        clampKirkleToBounds(&x, &y, &vx, &vy, radius);
        clampSquareToBounds(&sq);

        renderr(renderer, x, y, radius, &sq);
        SDL_Delay(1);
    }

    cleanup(window, renderer);
    return 0;
}
