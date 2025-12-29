#ifndef INPUT_DRAG_H
#define INPUT_DRAG_H

#include <stdbool.h>
#include "physics.h"

typedef enum { DRAG_NONE, DRAG_KIRKLE, DRAG_SQUARE } DragTarget;

bool pointInKirkle(int mx, int my, float cx, float cy, int r);
bool pointInSquare(int mx, int my, const Square *s);

void handleEvents(bool *running,
                  DragTarget *dragTarget,
                  float *grabOffsetX, float *grabOffsetY,
                  float *cx, float *cy, float *cvx, float *cvy, int cr,
                  Square *square);

#endif
