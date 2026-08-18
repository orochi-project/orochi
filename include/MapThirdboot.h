#pragma once

#include "Notes.h"
#include "gbdk/platform.h"

/** The total frame count of the beatmap. */
#define MAP_THIRDBOOT_FRAME_COUNT 8707
/** The number of notes in the beatmap. */
#define MAP_THIRDBOOT_NOTE_COUNT 216

BANKREF_EXTERN(map_thirdboot)

/** The array of notes in the beatmap. */
extern const Note map_thirdboot_notes[MAP_THIRDBOOT_NOTE_COUNT];
