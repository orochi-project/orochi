#pragma once

#include "Notes.h"
#include <stdbool.h>
#include <stdint.h>

/** The index of the currently selected map. */
extern uint8_t selected_map_idx;

/** The amount of health points the player has (max: 1000). */
extern uint16_t player_health;

/** The counts of notes graded so far in the current attempt. */
extern uint16_t hit_grade_counts[4];

/** The current/latest note hit grade. */
extern NoteHitGrade latest_hit_grade;

/** Whether or not the HUD hit grade label should be redrawn. */
extern bool should_draw_hit_grade_label;
