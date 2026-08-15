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

/** The counter to track when one audio tick must be skipped. */
static uint8_t skip_counter = 0;

/** The last recorded accuracy percent. */
static uint8_t last_accuracy_percent = 100;

void PlayCurrentMapSong(void) BANKED {
    const Map *selected_map = &maps[selected_map_idx];
    audio_skip_interval = selected_map->audio_skip_interval;
    __PlayMusic((void *)selected_map->song, selected_map->song_bank, true);
}

void InitGameAudio(void) BANKED {
    sfx_sound_init();
    sfx_sound_cut();

    skip_counter = 0;

    CRITICAL {
        TAC_REG = 0x00;       // stop timer
        IE_REG &= ~TIM_IFLAG; // disable timer interrupt mask
        IF_REG &= ~TIM_IFLAG; // clear timer interrupts

        set_interrupts(IE_REG | VBL_IFLAG);
    }

    INIT_MUSIC_DRIVER();
}

void RestoreDefaultAudio(void) BANKED {
    CRITICAL {
        TAC_REG = 0x04;
        IF_REG &= ~TIM_IFLAG;
        set_interrupts(IE_REG | TIM_IFLAG);
    }
}

void TickGameAudio(void) NONBANKED {
    if (music_paused)
        return;
    if (last_music_bank == SFX_STOP_BANK)
        return;

    // skip once every audio_skip_interval ticks
    if (audio_skip_interval)
        if (++skip_counter >= audio_skip_interval) {
            skip_counter = 0;
            return; // skip to slow down audio
        }

    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(last_music_bank);

    hUGE_dosound();

    SWITCH_ROM(_saved_bank);
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
