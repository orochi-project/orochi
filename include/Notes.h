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

/** Bitfield flags shared by all note sprite data types. */
typedef struct {
    bool scanline_speed_changed : 1;   ///< Whether or not the scanline's
                                       ///< velocity has been changed to the
                                       ///< note's speed modifier.
    bool scanline_snapped : 1;         ///< Whether or not the scanline has
                                       ///< already been snapped to the note's
                                       ///< assigned x-position and direction.
    bool note_hit : 1;                 ///< Whether or not the player
                                       ///< successfully hit the note.
    bool note_pending_destruction : 1; ///< Whether or not the note has been
                                       ///< flagged for destruction after
                                       ///< being missed. The note remains
                                       ///< on screen for a short duration
                                       ///< once this is set as a visual cue
                                       ///< that the note was missed.
    bool note_holding : 1;             ///< Whether or not the hold note is
                                       ///< currently in its active holding
                                       ///< state.
    bool note_hold_locked : 1;         ///< Whether or not the hold note's hold
                                       ///< has ended. The hold note cannot
                                       ///< re-enter an active hold when this
                                       ///< flag is set.
    bool note_hold_armed : 1;          ///< Whether or not this note is armed
                                       ///< to start holding as soon as it's
                                       ///< able to.
} NoteFlags;

/** Sprite custom data for tap notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint16_t current_frame;
    uint8_t scanline_x;
    int8_t scanline_direction;
    NoteFlags flags;
} TapNoteData;

/** Sprite custom data for reverse notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint16_t current_frame;
    uint8_t scanline_x;
    int8_t scanline_direction;
    NoteFlags flags;
} ReverseNoteData;

/** Sprite custom data for hold notes. */
typedef struct {
    uint8_t speed_modifier;
    uint8_t charge_frames;
    uint8_t hold_frames;
    uint16_t current_frame;
    uint8_t frames_missed;
    uint8_t scanline_x;
    int8_t scanline_direction;
    NoteFlags flags;
} HoldNoteData;

/** Types of note hits. */
typedef enum {
    HitPerfect, ///< A perfect hit.
    HitEarly,   ///< An early hit.
    HitLate,    ///< A late hit.
    HitMiss,    ///< A missed note.
} NoteHitGrade;
