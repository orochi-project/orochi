#pragma once

#include "Notes.h"
#include <stdbool.h>
#include <stdint.h>

/** The index of the currently selected map. */
extern uint8_t selected_map_idx;

/** The amount of health points the player has (max: 1000). */
extern uint16_t player_health;

/** The current/latest note hit grade. */
extern NoteHitGrade latest_hit_grade;

/** Whether or not the hit grade label should be redraw in the HUD. */
extern bool should_draw_hit_grade_label;

/** Play the song for the currently selected map. */
void PlayCurrentMapSong(void);
