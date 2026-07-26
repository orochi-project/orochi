#pragma once

#include <stdint.h>

/** The number of levels to be defined. */
#define LEVEL_COUNT 4

/** A structure representing a single level/map. */
struct Level {
    uint8_t id;         ///< The level ID/number.
    char name[11];      ///< The name of the level (maximum: 10 characters).
    uint8_t difficulty; ///< The difficulty of the level (1-5).
    // name and difficulty has +1 than what is needed for \0 room.
};

/** The list of levels in the game. */
extern const struct Level
    levels[LEVEL_COUNT]; ///< Get number of levels using levels_length.
