#include "Banks/SetAutoBank.h"

#include "GameAudio.h"
#include "GameData.h"
#include "GameStore.h"
#include "Maps.h"
#include "Music.h"

/** The counter to track when one audio tick must be skipped. */
static uint8_t skip_counter = 0;

/** The last recorded accuracy percent. */
static uint8_t last_accuracy_percent = 100;

/** The accuracy weight per hit grade (perfect, early, late, and miss). */
static const uint8_t grade_weight[4] = {100, 50, 50, 0};

/** The cumulative weighted score. */
static uint32_t total_weighted_score = 0;
/** The total count of graded notes. */
static uint16_t total_graded_notes = 0;

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

void RegisterNoteHit(NoteHitGrade grade) BANKED {
    latest_hit_grade = grade;
    should_draw_hit_grade_label = true;

    total_weighted_score += grade_weight[grade];
    ++total_graded_notes;

    last_accuracy_percent =
        (uint8_t)(total_weighted_score / total_graded_notes);
}

void ResetAccuracy(void) BANKED {
    total_weighted_score = 0;
    total_graded_notes = 0;
    last_accuracy_percent = 100;
}

uint8_t GetAccuracyPercent(void) BANKED { return last_accuracy_percent; }
