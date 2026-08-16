#pragma once

#include "Sprite.h"
#include <stdbool.h>

/** The starting x-position of the scanline. */
#define SCANLINE_START_X (5 + 2)
/** The starting y-position of the scanline. */
#define SCANLINE_START_Y 8

/** The x-position of the left boundary of the scanline. */
#define SCANLINE_BOUND_LEFT_X (5 + 2)
/** The x-position of the right boundary of the scanline. */
#define SCANLINE_BOUND_RIGHT_X (147 + 2)

/** The width of the scanline collision box. */
#define SCANLINE_COLLISION_WIDTH 4

/** The starting velocity of the scanline. */
#define SCANLINE_START_VELOCITY 1

/** Sprite custom data for the scanline. */
typedef struct {
    int8_t velocity;
    bool frozen;
} ScanlineData;

/** A pointer to the scanline sprite. */
extern Sprite *scanline_sprite;
