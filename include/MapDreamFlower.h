#pragma once

#include "Notes.h"
#include "gbdk/platform.h"

/** The total frame count of the beatmap. */
#define MAP_DREAM_FLOWER_FRAME_COUNT 13931
/** The number of notes in the beatmap. */
#define MAP_DREAM_FLOWER_NOTE_COUNT 161

BANKREF_EXTERN(map_dream_flower)

/** The array of notes in the beatmap. */
extern const Note map_dream_flower_notes[MAP_DREAM_FLOWER_NOTE_COUNT];
