#pragma once

#include "Notes.h"
#include "gbdk/platform.h"

/** The total frame count of the beatmap. */
#define MAP_ISOLATION_FRAME_COUNT 11610
/** The number of notes in the beatmap. */
#define MAP_ISOLATION_NOTE_COUNT 53

BANKREF_EXTERN(map_isolation)

/** The array of notes in the beatmap. */
extern const Note map_isolation_notes[MAP_ISOLATION_NOTE_COUNT];
