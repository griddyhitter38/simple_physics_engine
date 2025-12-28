#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define HEIGHT 1000
#define WIDTH  800

void drawFilledKirkle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        int y = cy + dy;
        double t = (double)radius * radius - (double)dy * dy;
        int dx = (int)sqrt(t);
        SDL_RenderDrawLine(renderer, cx - dx, y, cx + dx, y);
    }
}

// init sdl
bool init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN,
                                    window, renderer) != 0)
        return false;
    return true;
}

// events
void handleEvents(bool *running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) *running = false;
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            *running = false;
    }
}

// dt
float getDeltaTime(Uint32 *lastTime) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - *lastTime) / 1000.0f;
    *lastTime = currentTime;
    if (deltaTime > 0.05f) deltaTime = 0.05f;
    return deltaTime;
}

// physics
void updatePhysics(
    float *x, float *y,
    float *vx, float *vy,
    float *ax, float *ay,
    int radius,
    float deltaTime
) {
    // drag
    const float drag = 0.98f;

    // control accel
    const float inputAccel = 20000.0f;

    // gravity
    const float g0 = 1000.0f;
    const float k  = 9.81f;
    const float ayLimit = 40000.0f;
    static float y0 = 0.0f;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    *ax = 0.0f;
    *ay = 0.0f;

    if (keys[SDL_SCANCODE_LEFT])  *ax -= inputAccel;
    if (keys[SDL_SCANCODE_RIGHT]) *ax += inputAccel;
    if (keys[SDL_SCANCODE_UP])    *ay -= inputAccel;
    if (keys[SDL_SCANCODE_DOWN])  *ay += inputAccel;

    float dy = *y - y0;
    if (dy < 0.0f) dy = 0.0f;

    float gravityAy = g0 + k * dy;
    if (gravityAy > ayLimit) gravityAy = ayLimit;

    *ay += gravityAy;

    // vel -> acc
    *vx += *ax * deltaTime;
    *vy += *ay * deltaTime;

    // drag on vel
    *vx *= drag;
    *vy *= drag;

    // vel -> pos
    *x += *vx * deltaTime;
    *y += *vy * deltaTime;

    //bounce
    const float restitution = 0.99f;

    // collision wall
    if (*x < radius) {
        *x = radius;
        if (*vx < 0.0f)
            *vx = -(*vx) * restitution;
    }
    if (*x > WIDTH - radius) {
        *x = WIDTH - radius;
        if (*vx > 0.0f)
            *vx = -(*vx) * restitution;
    }

    // collision up/down
    if (*y < radius) {
        *y = radius;
        if (*vy < 0.0f)
            *vy = -(*vy) * restitution;
    }
    if (*y > HEIGHT - radius) {
        *y = HEIGHT - radius;
        if (*vy > 0.0f)
            *vy = -(*vy) * restitution;
    }
}

// render
void render(SDL_Renderer *renderer, float x, float y, int radius) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawFilledKirkle(renderer, (int)x, (int)y, radius);

    SDL_RenderPresent(renderer);
}

// cleanup
void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer)) return 1;

    // kirle apperance
    int radius = 70;

    // physics state
    float x = WIDTH / 2.0f;
    float y = HEIGHT / 2.0f;
    float vx = 0.0f;
    float vy = 0.0f;

    // acceleration
    float ax = 0.0f;
    float ay = 0.0f;

    Uint32 lastTime = SDL_GetTicks();

    // kirle movement
    while (running) {
        handleEvents(&running);
        float deltaTime = getDeltaTime(&lastTime);
        updatePhysics(&x, &y, &vx, &vy, &ax, &ay, radius, deltaTime);
        render(renderer, x, y, radius);
        SDL_Delay(1);
    }

    cleanup(window, renderer);
    return 0;
}
