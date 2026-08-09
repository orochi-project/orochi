#pragma once

#include "Notes.h"
#include "gbdk/platform.h"
#include <stdint.h>

BANKREF_EXTERN(map_dream_flower)

Note MapDreamFlowerGetNote(uint16_t note_idx) NONBANKED;
uint16_t MapDreamFlowerGetNoteCount(void) NONBANKED;
