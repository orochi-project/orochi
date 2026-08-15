#include "Banks/SetAutoBank.h"

#include "GameAudio.h"
#include "GameData.h"
#include "GameStore.h"
#include "Maps.h"
#include "Music.h"

/** The accuracy weighting for perfect note hits. */
#define NOTE_HIT_PERFECT_WEIGHT 100
/** The accuracy weighting for early note hits. */
#define NOTE_HIT_EARLY_WEIGHT 50
/** The accuracy weighting for late note hits. */
#define NOTE_HIT_LATE_WEIGHT 50

/** The last recorded accuracy percent. */
static uint8_t last_accuracy_percent = 100;

void PlayCurrentMapSong(void) BANKED {
    const Map *selected_map = &maps[selected_map_idx];
    audio_skip_interval = selected_map->audio_skip_interval;
    __PlayMusic((void *)selected_map->song, selected_map->song_bank, true);
}

static void RecalculateAccuracyPercent(void) {
    uint16_t graded_note_count =
        hit_grade_counts[HitPerfect] + hit_grade_counts[HitEarly] +
        hit_grade_counts[HitLate] + hit_grade_counts[HitMiss];
    // 100% if no notes have been graded
    if (graded_note_count == 0) {
        last_accuracy_percent = 100;
        return;
    }

    uint32_t weighted_score =
        (uint32_t)hit_grade_counts[HitPerfect] * NOTE_HIT_PERFECT_WEIGHT +
        (uint32_t)hit_grade_counts[HitEarly] * NOTE_HIT_EARLY_WEIGHT +
        (uint32_t)hit_grade_counts[HitLate] * NOTE_HIT_LATE_WEIGHT;

    last_accuracy_percent = (uint8_t)(weighted_score / graded_note_count);
}

void RegisterNoteHit(NoteHitGrade grade) BANKED {
    latest_hit_grade = grade;
    should_draw_hit_grade_label = true;
    ++hit_grade_counts[grade];
    RecalculateAccuracyPercent();
}

void ResetAccuracy(void) BANKED {
    for (uint8_t i = 0; i < 4; ++i)
        hit_grade_counts[i] = 0;
    last_accuracy_percent = 100;
}

uint8_t GetAccuracyPercent(void) BANKED { return last_accuracy_percent; }
