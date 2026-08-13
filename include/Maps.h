#pragma once

#include "hUGEDriver.h"
#include <stdint.h>

/** The number of maps to be defined. */
#define MAP_COUNT 4

/** The maximum length of the map name text (including '\0'). */
#define MAP_NAME_MAX_LENGTH 13

/** A single map. */
typedef struct {
    uint8_t id;                     ///< The map ID/number.
    char name[MAP_NAME_MAX_LENGTH]; ///< The name of the map.
    uint8_t difficulty;             ///< The difficulty of the map (1-5).
    hUGESong_t *song;               ///< The song to play for the map.
    uint8_t song_bank;              ///< The song's ROM bank.
    uint16_t audio_skip_interval; ///< The frame interval over which exactly one
                                  ///< audio tick should be skipped.
} Map;

/** The list of maps in the game. */
extern const Map maps[MAP_COUNT];
