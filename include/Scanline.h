#pragma once

#include "Sprite.h"

/** The starting x-position of the scanline. */
#define SCANLINE_START_X 8
/** The starting y-position of the scanline. */
#define SCANLINE_START_Y 64

/** The x-position of the left boundary of the scanline. */
#define SCANLINE_BOUND_LEFT_X 9
/** The x-position of the right boundary of the scanline. */
#define SCANLINE_BOUND_RIGHT_X 151

/** The starting velocity of the scanline. */
#define SCANLINE_START_VELOCITY 1

/** Sprite custom data for the scanline. */
typedef struct {
    int8_t velocity;
} ScanlineData;

/** A pointer to the scanline sprite. */
extern Sprite *scanline_sprite;
