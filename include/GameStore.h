#pragma once

#include "Notes.h"
#include "Sprite.h"
#include "ZGBMain.h"
#include <stdint.h>

/** Play the song for the currently selected map. */
void PlayCurrentMapSong(void) BANKED;

/** Initialize the audio for the game. */
void InitGameAudio(void) BANKED;

/** Restore the default CrossZGB music settings. */
void RestoreDefaultAudio(void) BANKED;

/** Step forward by one tick in the currently loaded audio. */
void TickGameAudio(void) NONBANKED;

/**
 * Record a note's hit grade.
 *
 * @param grade  The grade the note was graded with.
 */
void RegisterNoteHit(NoteHitGrade grade) BANKED;

/** Reset all hit grade counts back to zero. */
void ResetAccuracy(void) BANKED;

/**
 * Calculate the priority score for a note.
 *
 * A lower priority score means a higher priority over other notes.
 */
static inline uint16_t GetNotePriority(Sprite *note_sprite,
                                       uint8_t late_threshold) {
    uint16_t current_frame = 0;
    uint8_t charge_frames = 0;

    switch (note_sprite->type) {
    case SpriteReverseNote: {
        ReverseNoteData *note_data =
            (ReverseNoteData *)note_sprite->custom_data;
        current_frame = note_data->current_frame;
        charge_frames = note_data->charge_frames;
        break;
    }
    case SpriteTapNoteHorizontal:
    case SpriteTapNoteVertical: {
        TapNoteData *note_data = (TapNoteData *)note_sprite->custom_data;
        current_frame = note_data->current_frame;
        charge_frames = note_data->charge_frames;
        break;
    }
    case SpriteHoldNote: {
        HoldNoteData *note_data = (HoldNoteData *)note_sprite->custom_data;
        current_frame = note_data->current_frame;
        charge_frames = note_data->charge_frames;
        break;
    }
    default:
        return 9999;
    }

    if (current_frame > charge_frames + late_threshold)
        return 0; // late
    else if (current_frame >= charge_frames ||
             note_sprite->anim_frame == NOTE_CHARGE_FRAME_COUNT - 1)
        return 1; // fully charged
    else
        return 2 + (charge_frames - current_frame); // early
}

/**
 * Determine whether or not a given momentary (tap or reverse) note is the
 * highest-priority note to be hit in its column.
 */
bool IsBestMomentaryNoteForColumn(Sprite *note_sprite,
                                  uint8_t late_threshold) BANKED;

/**
 * Determine whether or not a given hold note is the highest-priority note to be
 * armed in its column.
 */
bool IsBestHoldNoteForColumn(Sprite *note_sprite) BANKED;
