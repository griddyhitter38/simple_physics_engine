#include "headers/physics.h"
#include <math.h>

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void updatePhysics(float *x, float *y,
                   float *vx, float *vy,
                   float *ax, float *ay,
                   int radius, float dt)
{
    const float drag = 0.98f;
    const float inputAccel = 20000.0f;
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

    *vx += *ax * dt;
    *vy += *ay * dt;

    *vx *= drag;
    *vy *= drag;

    *x += *vx * dt;
    *y += *vy * dt;
}

void updateSquare(Square *s, float dt)
{
    const float drag = 0.98f;
    const float inputAccel = 20000.0f;
    const float g0 = 1000.0f;
    const float k  = 9.81f;
    const float ayLimit = 40000.0f;
    static float y0 = 0.0f;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    s->ax = 0.0f;
    s->ay = 0.0f;

    if (keys[SDL_SCANCODE_A]) s->ax -= inputAccel;
    if (keys[SDL_SCANCODE_D]) s->ax += inputAccel;
    if (keys[SDL_SCANCODE_W]) s->ay -= inputAccel;
    if (keys[SDL_SCANCODE_S]) s->ay += inputAccel;

    float dy = s->y - y0;
    if (dy < 0.0f) dy = 0.0f;

    float gravityAy = g0 + k * dy;
    if (gravityAy > ayLimit) gravityAy = ayLimit;

    s->ay += gravityAy;

    s->vx += s->ax * dt;
    s->vy += s->ay * dt;

    s->vx *= drag;
    s->vy *= drag;

    s->x += s->vx * dt;
    s->y += s->vy * dt;
}

void resolveKirkleSquareCollision(float *cx, float *cy,
                                  float *cvx, float *cvy,
                                  int r, Square *sq)
{
    float left   = sq->x - sq->half;
    float right  = sq->x + sq->half;
    float top    = sq->y - sq->half;
    float bottom = sq->y + sq->half;

    float closestX = clampf(*cx, left, right);
    float closestY = clampf(*cy, top, bottom);

    float dx = *cx - closestX;
    float dy = *cy - closestY;

    float dist2 = dx*dx + dy*dy;
    float rF = (float)r;

    if (dist2 >= rF*rF) return;

    float dist = sqrtf(dist2);

    float nx = (dist > 0.0f) ? dx / dist : 0.0f;
    float ny = (dist > 0.0f) ? dy / dist : 1.0f;

    float penetration = rF - dist;

    *cx += nx * penetration * 0.5f;
    *cy += ny * penetration * 0.5f;
    sq->x -= nx * penetration * 0.5f;
    sq->y -= ny * penetration * 0.5f;

    float rvx = *cvx - sq->vx;
    float rvy = *cvy - sq->vy;

    float velAlongNormal = rvx*nx + rvy*ny;
    if (velAlongNormal > 0.0f) return;

    const float restitution = 0.6f;
    float j = -(1.0f + restitution) * velAlongNormal * 0.5f;

    *cvx += j * nx;
    *cvy += j * ny;
    sq->vx -= j * nx;
    sq->vy -= j * ny;
}

void clampKirkleToBounds(float *x, float *y, float *vx, float *vy, int r)
{
    if (*x < r) { *x = r; if (*vx < 0.0f) *vx = -*vx; }
    if (*x > WIDTH - r) { *x = WIDTH - r; if (*vx > 0.0f) *vx = -*vx; }
    if (*y < r) { *y = r; if (*vy < 0.0f) *vy = -*vy; }
    if (*y > HEIGHT - r) { *y = HEIGHT - r; if (*vy > 0.0f) *vy = -*vy; }
}

void clampSquareToBounds(Square *s)
{
    if (s->x < s->half) { s->x = s->half; if (s->vx < 0.0f) s->vx = -s->vx; }
    if (s->x > WIDTH - s->half) { s->x = WIDTH - s->half; if (s->vx > 0.0f) s->vx = -s->vx; }
    if (s->y < s->half) { s->y = s->half; if (s->vy < 0.0f) s->vy = -s->vy; }
    if (s->y > HEIGHT - s->half) { s->y = HEIGHT - s->half; if (s->vy > 0.0f) s->vy = -s->vy; }
}
