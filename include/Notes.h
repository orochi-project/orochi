#pragma once

#include <gbdk/platform.h>
#include <stdbool.h>
#include <stdint.h>

/** The pixel-based width of a note. */
#define NOTE_WIDTH 16
/** The pixel-based height of a note. */
#define NOTE_HEIGHT 16

/** The number of grid rows available. */
#define GRID_ROWS 5
/** The number of grid columns available. */
#define GRID_COLS 8
/** The pixel-based x-position of the first grid cell, anchored on the left
 * edge. */
#define GRID_START_X 16
/** The pixel-based y-position of the first grid cell, anchored on the top edge.
 */
#define GRID_START_Y 20

#define FLAG_SPEED_CHANGED 0x01
#define FLAG_SNAPPED 0x02
#define FLAG_REVERSED 0x04
#define FLAG_CLICKED 0x08
#define FLAG_DESTROY_PENDING 0x10

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
    NoteType type;          ///< The type of note.
    uint8_t grid_idx;       ///< The map grid index to place the note.
    uint8_t speed_modifier; ///< The speed modifier of the note.
    uint16_t
        appear_frame; ///< The frame on which the note will appear (0-65535).
    uint8_t charge_frames; ///< The number of frames to wait before a note
                           ///< should be pressed (0-255).
    uint8_t
        hold_frames; ///< The number of frames to hold a note for. Only applies
                     ///< to hold notes (0-255); all other notes should take 0.
    uint8_t scanline_x; ///< The x-position of the scanline on the peak frame of
                        ///< the note.
    int8_t scanline_direction; ///< The direction of the scanline on the peak
                               ///< frame of the note.
} Note;

/** Sprite custom data for tap notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint16_t current_frame;
    uint8_t scanline_x;
    int8_t scanline_direction;
    uint8_t flags;
} TapNoteData;

/** Sprite custom data for reverse notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint16_t current_frame;
    uint8_t scanline_x;
    int8_t scanline_direction;
    uint8_t flags;
} ReverseNoteData;

/** Sprite custom data for hold notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint8_t hold_frames;
    uint16_t current_frame;
    uint8_t scanline_x;
    int8_t scanline_direction;
    uint8_t flags;
} HoldNoteData;
