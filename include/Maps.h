#pragma once

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
} Map;

/** The list of maps in the game. */
extern const Map maps[MAP_COUNT];

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

/** A note in a map. */
typedef struct {
    NoteType note_type; ///< The type of note.
    uint8_t grid_idx;   ///< The map grid index to place the note (0-39).
    uint16_t
        appear_frame; ///< The frame on which the note will appear (0-65535).
    uint8_t charge_frames; ///< The number of frames to wait before a note
                           ///< should be pressed (0-255).
    uint8_t
        hold_frames; ///< The number of frames to hold a note for. Only applies
                     ///< to hold notes (0-255); all other notes should take 0.
} Note;
