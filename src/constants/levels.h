#pragma once

#include <stdint.h>

#define levels_length 4

/** A structure representing a single level/map. */
struct Level {
    uint8_t id;         ///< The level ID/number.
    char name[9];       ///< The name of the level (maximum: 8 characters). 
    char difficulty[6]; ///< The difficulty of the level (1-5).
    //name and difficulty has +1 than what is needed for \0 room.
};

/** The list of levels in the game. */
extern const struct Level levels[levels_length]; ///< Get number of levels using levels_length.
