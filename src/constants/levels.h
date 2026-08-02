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
extern const struct Level levels[LEVEL_COUNT];

/** A structure representing each note in a level. */
typedef struct {
    uint8_t note_type;          ///<Type of note; includes direction of note.
    uint8_t grid_x;             ///< The grid X position (0-19).
    uint8_t grid_y;             ///< The grid Y position (0-17).
    uint16_t appear_frames;     ///< The frame that the note will appear (0-65535).
    uint8_t charge_frames;      ///< How much frames to wait before a note should be pressed (0-255).
    uint8_t hold_frames;        ///< How much frames to hold a note for; applies only for hold notes (0-255).
} Note;

