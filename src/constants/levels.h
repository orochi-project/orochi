#pragma once

#include <stdint.h>

/** The number of levels to be defined. */
#define LEVEL_COUNT 4

/** A single level/map. */
typedef struct {
    uint8_t id;         ///< The level ID/number.
    char name[11];      ///< The name of the level (maximum: 10 characters).
    uint8_t difficulty; ///< The difficulty of the level (1-5).
} Level;

/** The list of levels in the game. */
extern const Level levels[LEVEL_COUNT];

/** A note type. */
typedef enum {
    TapLeft,  ///< A tap note directed to the left.
    TapRight, ///< A tap note directed to the right.
    TapUp,    ///< A tap note directed upward.
    TapDown,  ///< A tap note directed downward.
    Hold,     ///< A hold note (activated with the A key). Freezes the scanline
              ///< until the hold ends.
    Reverse,  ///< A reverse note (activated with the B key). Reverses the
              ///< direction of the scanline.
} NoteType;

/** A note in a level. */
typedef struct {
    NoteType note_type; ///< The type of note.
    uint8_t grid_x;     ///< The grid x-position (0-19).
    uint8_t grid_y;     ///< The grid y-position (0-17).
    uint16_t
        appear_frame; ///< The frame on which the note will appear (0-65535).
    uint8_t charge_frames; ///< The number of frames to wait before a note
                           ///< should be pressed (0-255).
    uint8_t
        hold_frames; ///< The number of frames to hold a note for. Only applies
                     ///< to hold notes (0-255); all other notes should take 0.
} Note;
