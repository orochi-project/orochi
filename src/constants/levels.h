#pragma once

#include <stdint.h>

/** The number of levels to be defined. */
#define LEVEL_COUNT 4

/** A structure representing a single level/map. */
struct Level {
    uint8_t id;         ///< The level ID/number.
    char name[11];      ///< The name of the level (maximum: 10 characters).
    uint8_t difficulty; ///< The difficulty of the level (1-5).
};

/** The list of levels in the game. */
extern const struct Level
    levels[LEVEL_COUNT];
