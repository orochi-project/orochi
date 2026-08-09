#pragma once

#include <stdint.h>

/** The index of the currently selected map. */
extern uint8_t selected_map_idx;

/** Play the song for the currently selected map. */
void PlayCurrentMapSong(void);
