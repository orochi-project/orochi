#pragma once

#include "hUGEDriver.h"
#include <stdint.h>

/** The number of maps to be defined. */
#define MAP_COUNT 4

/** The maximum length of the map name text (including '\0'). */
#define MAP_NAME_MAX_LENGTH 13

/** A single map. */
typedef struct {
    const uint8_t id;                     ///< The map ID/number.
    const char name[MAP_NAME_MAX_LENGTH]; ///< The name of the map.
    const uint8_t difficulty;             ///< The difficulty of the map (1-5).
    const hUGESong_t *song;
    const uint8_t song_bank;
} Map;

/** The list of maps in the game. */
extern const Map maps[MAP_COUNT];
