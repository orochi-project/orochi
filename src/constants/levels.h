#pragma once

#include <stdint.h>

/** A structure representing a single level/map. */
struct Level {
    uint8_t id;         ///< The level ID/number.
    char name[8];       ///< The name of the level (maximum: 8 characters).
    uint8_t difficulty; ///< The difficulty of the level (1-5).
};

/** The list of levels in the game. */
extern const struct Level levels[3];
