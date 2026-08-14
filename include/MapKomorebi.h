#pragma once

#include "Notes.h"
#include "gbdk/platform.h"

/** The total frame count of the beatmap. */
#define MAP_KOMOREBI_FRAME_COUNT 11738
/** The number of notes in the beatmap. */
#define MAP_KOMOREBI_NOTE_COUNT 104

BANKREF_EXTERN(map_komorebi)

/** The array of notes in the beatmap. */
extern const Note map_komorebi_notes[MAP_KOMOREBI_NOTE_COUNT];
