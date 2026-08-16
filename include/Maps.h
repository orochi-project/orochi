#pragma once

#include "Notes.h"
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
    const Note *notes;            ///< The array of notes for the map.
    uint16_t note_count;          ///< The number of notes in the map.
    uint8_t
        notes_bank; ///< The bank in which the notes for the map are located.
    uint16_t frame_count; ///< The total number of frames in the map.
} Map;

/** The list of maps in the game. */
extern const Map maps[MAP_COUNT];

/**
 * Fetch a note out of a banked map's note array.
 *
 * @param notes         A pointer to the note array.
 * @param notes_bank    The ROM bank the note array lives in.
 * @param note_idx      The index of the note to fetch.
 *
 * @return The requested note, copied out of ROM.
 */
Note GetMapNote(const Note *notes, uint8_t notes_bank, uint16_t note_idx);
