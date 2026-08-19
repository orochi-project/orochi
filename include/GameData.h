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

/** Whether or not the HUD hit grade label should be redrawn. */
extern bool should_draw_hit_grade_label;

/** The last recorded accuracy percent. */
extern uint8_t last_accuracy_percent;

/** The number of notes the player hit perfectly, early, late. */
extern uint16_t notes_hit_count;

/** Sprite custom data for the letter grade icon. */
typedef struct {
    unsigned char letter; ///< The letter to display.
} LetterGradeData;
