#include "Banks/SetAutoBank.h"

#include "GameAudio.h"
#include "GameData.h"
#include "GameStore.h"
#include "Maps.h"
#include "Music.h"
#include "Notes.h"
#include "Scanline.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include <stdint.h>

/** The counter to track when one audio tick must be skipped. */
static uint8_t skip_counter = 0;

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

bool IsBestMomentaryNoteForColumn(Sprite *note_sprite,
                                  uint8_t late_threshold) BANKED {
    uint16_t note_priority = GetNotePriority(note_sprite, late_threshold);
    uint8_t active_note_count =
        sprite_manager_updatables[0]; // first element is the count

    for (uint8_t i = 1; i <= active_note_count; ++i) {
        uint8_t sprite_idx = sprite_manager_updatables[i];
        Sprite *sprite = sprite_manager_sprites[sprite_idx];
        // only check notes that are different and are active
        if (!sprite || sprite == note_sprite)
            continue;

        // not applicable
        if (sprite->type != SpriteTapNoteHorizontal &&
            sprite->type != SpriteTapNoteVertical &&
            sprite->type != SpriteReverseNote)
            continue;

        // must be on same column
        if (sprite->x != note_sprite->x)
            continue;

        bool other_was_hit = false;
        if (sprite->type == SpriteReverseNote)
            other_was_hit =
                ((ReverseNoteData *)sprite->custom_data)->flags.note_hit;
        else
            other_was_hit =
                ((TapNoteData *)sprite->custom_data)->flags.note_hit;

        if (!other_was_hit && !CheckCollision(sprite, scanline_sprite))
            continue;

        uint16_t sprite_priority = GetNotePriority(sprite, late_threshold);

        if (sprite_priority < note_priority)
            return false;
    }

    // no notes in this column exist that are better candidates to be hit
    return true;
}

bool IsBestHoldNoteForColumn(Sprite *note_sprite) BANKED {
    uint16_t note_priority = GetNotePriority(note_sprite, 0);
    uint8_t active_note_count = sprite_manager_updatables[0];

    for (uint8_t i = 1; i <= active_note_count; ++i) {
        uint8_t sprite_idx = sprite_manager_updatables[i];
        Sprite *sprite = sprite_manager_sprites[sprite_idx];
        // only check notes that are different and are active
        if (!sprite || sprite == note_sprite)
            continue;

        if (sprite->type != SpriteHoldNote)
            continue;
        if (sprite->x != note_sprite->x)
            continue;

        HoldNoteData *sprite_data = (HoldNoteData *)sprite->custom_data;

        // already holding, or locked
        if (sprite_data->flags.note_holding ||
            sprite_data->flags.note_hold_locked)
            continue;

        uint16_t sprite_priority = GetNotePriority(sprite, 0);

        if (sprite_priority < note_priority)
            return false;
    }

    return true;
}
