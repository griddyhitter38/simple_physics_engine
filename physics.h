#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>

#define HEIGHT 1000
#define WIDTH  800

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    int half;
    Uint8 R, G, B, A;
} Square;

void updatePhysics(float *x, float *y,
                   float *vx, float *vy,
                   float *ax, float *ay,
                   int radius, float dt);

void updateSquare(Square *s, float dt);

void resolveKirkleSquareCollision(float *cx, float *cy,
                                  float *cvx, float *cvy,
                                  int r, Square *sq);

void clampKirkleToBounds(float *x, float *y, float *vx, float *vy, int r);
void clampSquareToBounds(Square *s);

#endif
