#pragma once

#include <stdbool.h>
#include <stdint.h>

/** The index of the currently selected map. */
extern uint8_t selected_map_idx;

/** The amount of health points the player has (max: 1000). */
extern uint16_t player_health;

/** Play the song for the currently selected map. */
void PlayCurrentMapSong(void);
