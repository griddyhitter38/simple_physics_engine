#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <bits/types/cookie_io_functions_t.h>
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include "stdbool.h"

#define HEIGHT 1000
#define WIDTH 800

void drawFilledKirkle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        int y = cy + dy;

        double t = (double)radius * radius - (double)dy * dy;
        int dx = (int)sqrt(t);

        int x1 = cx - dx;
        int x2 = cx + dx;

        SDL_RenderDrawLine(renderer, x1, y, x2, y);
    }
}

void drawKirkle(SDL_Renderer *renderer, int cx, int cy, int radius, int mass, float acceleration){
    int segments = 360;
    float gravity = acceleration * mass;

    for (int i = 0; i < segments; i++){
        double theta = i * (2.00 * M_PI / segments);
    int x = cx + (int)(radius * cos(theta));
    int y = cy + (int)(radius * sin(theta));
    SDL_RenderDrawPoint(renderer,x,y);

    }
}

int main(void){
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("init error: %s", SDL_GetError());
            return 1;
        }

    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) != 0) {
           SDL_Log("createwindow error: %s", SDL_GetError());
           SDL_Quit();
           return 1;
       }

    //kirle apperance
    const Uint8 R = 255,G = 255,B = 255,A = 255;
    int radius = 70;

    //physics state
    float x = WIDTH / 2.0f;
    float y = HEIGHT / 2.0f;
    float vx = 0.0f;
    float vy = 0.0f;

    //acceleration
    float ax = 0.0f;
    float ay = 0.0f;

    //drag
    const float drag = 0.98f;

    //control accel
    const float inputAccel = 20000.0f;


    //gravity
    float y0 = 0.0f;
    const float g0 = 1000.0f;
    const float k = 8.0f;
    const float ayLimit = 40000.0f;

    Uint32 lastTime = SDL_GetTicks();

    //kikr movement
    while (running){
        while (SDL_PollEvent(&event)) {

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;

        }

        //dt
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        ax = 0.0f;
        ay = 0.0f;

        if (keys[SDL_SCANCODE_LEFT]) ax -= inputAccel;
        if (keys[SDL_SCANCODE_RIGHT]) ax += inputAccel;
        if (keys[SDL_SCANCODE_UP]) ay -= inputAccel;
        if (keys[SDL_SCANCODE_DOWN]) ay += inputAccel;

        float dy = y - y0;
        if (dy < 0.0f) dy = 0.0f;

        float gravityAy = g0 + k * dy;
        if(gravityAy > ayLimit) gravityAy = ayLimit;

        ay += gravityAy;

        //vel -> acc
        vx += ax * deltaTime;
        vy += ay * deltaTime;

        // drag on vel
        vx *= drag;
        vy *= drag;
        //vel -> pos
        x += vx * deltaTime;
        y += vy * deltaTime;

        // collision wall
        if (x < radius){
            x = radius;
            vx = 0.0f;
        }
        if (x > WIDTH - radius){
            x = WIDTH - radius;
            vx = 0.0f;
        }

        // collision up/down
        if (y < radius){
            y = radius;
            vy = 0.0f;
        }
        if (y > HEIGHT - radius){
            y = HEIGHT - radius;
            vy = 0.0f;

        }
    SDL_SetRenderDrawColor(renderer, 0,0,0,A);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, R,G,B,A);
    drawFilledKirkle(renderer, (int)x, (int)y, radius);
    //drawKirkle(renderer, posx1, posy1, 100, 10, 0.1);

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
