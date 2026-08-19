#include "Banks/SetAutoBank.h"

#include "GameData.h"
#include "GameStore.h"
#include "Keys.h"
#include "MathUtils.h"
#include "Notes.h"
#include "Scanline.h"
#include "SpriteManager.h"
#include <stdbool.h>

/** The number of frames to allocate for the destruction timer. */
#define DESTRUCTION_FRAMES 30

/**
 * The number of frames after the note is charged before which a hit is marked
 * late.
 */
#define NOTE_HIT_LATE_THRESHOLD_FRAMES 5

/**
 * Update the charge animation frame of the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void UpdateChargeAnimation(TapNoteData *note_data);

/**
 * Check if the player hit the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void CheckPlayerClick(TapNoteData *note_data);

/**
 * Apply scanline modifiers for the note.
 *
 * @param note_data     A pointer to the note custom data.
 * @param scanline_data A pointer to the scanline custom data.
 */
static void ApplyScanlineModifiers(TapNoteData *note_data,
                                   ScanlineData *scanline_data);

/**
 * Handle the note's destruction.
 *
 * @param note_data A pointer to the note custom data.
 *
 * @return Whether or not the sprite was destroyed.
 */
static bool HandleDestruction(TapNoteData *note_data);

void START(void) {
    TapNoteData *note_data = (TapNoteData *)THIS->custom_data;
    note_data->current_frame = 0;
    memset(&note_data->flags, 0, sizeof(NoteFlags));
}

void UPDATE(void) {
    TapNoteData *note_data = (TapNoteData *)THIS->custom_data;
    ScanlineData *scanline_data = (ScanlineData *)scanline_sprite->custom_data;

    ++note_data->current_frame;

    UpdateChargeAnimation(note_data);
    CheckPlayerClick(note_data);
    ApplyScanlineModifiers(note_data, scanline_data);

    if (HandleDestruction(note_data))
        return;
}

void DESTROY(void) {}

static void UpdateChargeAnimation(TapNoteData *note_data) {
    // If the note is still in the charging stage, calculate the note frame
    // index and set the note animation frame to that index.
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t frame_idx =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_CHARGE_FRAME_COUNT - 1);
        SetFrame(THIS, frame_idx);
    }
}

static void CheckPlayerClick(TapNoteData *note_data) {
    if (note_data->flags.note_hit || !CheckCollision(THIS, scanline_sprite))
        return;

    // If the note was not already clicked and was just ticked this frame, move
    // the note off-screen and flag the note as clicked to prevent any later
    // clicks on this note from registering.
    bool is_mirrored = (THIS->mirror != 0);
    bool correct_key_pressed =
        is_mirrored ? KEY_TICKED(J_LEFT) : KEY_TICKED(J_RIGHT);

    if (!correct_key_pressed)
        return;

    // If the note is not the highest-priority note in its column, do not
    // register the click yet.
    if (!IsBestMomentaryNoteForColumn(THIS, NOTE_HIT_LATE_THRESHOLD_FRAMES))
        return;

    // If the note was hit past the lateness threshold, it was hit late.
    if (note_data->current_frame >
        note_data->charge_frames + NOTE_HIT_LATE_THRESHOLD_FRAMES)
        RegisterNoteHit(HitLate);
    // If the note was not hit past the lateness threshold, but was correctly
    // hit when the note was on its final frame, mark it as a perfect hit.
    else if (THIS->anim_frame == NOTE_CHARGE_FRAME_COUNT - 1)
        RegisterNoteHit(HitPerfect);
    // Otherwise, if the note was hit before its final frame, mark it as an
    // early hit.
    else
        RegisterNoteHit(HitEarly);

    note_data->flags.note_hit = true;

    should_draw_hit_grade_label = true;

    THIS->y = 144; // move below visible screen
}

static void ApplyScanlineModifiers(TapNoteData *note_data,
                                   ScanlineData *scanline_data) {
    // The scanline should only ever be modified after the note has passed its
    // charging stage.
    if (note_data->current_frame < note_data->charge_frames)
        return;

    // If the scanline was not already snapped to the note's assigned scanline
    // x-position and direction, snap it now.
    if (!note_data->flags.scanline_snapped) {
        scanline_sprite->x = SCANLINE_BOUND_LEFT_X + note_data->scanline_x;
        if ((scanline_data->velocity < 0) !=
            (note_data->scanline_direction < 0))
            scanline_data->velocity = -scanline_data->velocity;
        note_data->flags.scanline_snapped = true;
    }

    // If we did not already change the velocity of the scanline to the note's
    // speed modifier, change it now.
    // If the note's speed modifier is 0 (or anything falsy), do not modify the
    // scanline's velocity. The scanline's direction stays the same.
    if (!note_data->flags.scanline_speed_changed && note_data->speed_modifier) {
        scanline_data->velocity =
            SIGN(scanline_data->velocity) * note_data->speed_modifier;
        note_data->flags.scanline_speed_changed = true;
    }
}

static bool HandleDestruction(TapNoteData *note_data) {
    if (note_data->current_frame < note_data->charge_frames)
        return false;

    // If the note was clicked, destroy the note.
    if (note_data->flags.note_hit) {
        SpriteManagerRemoveSprite(THIS);
        return true;
    }

    // If the note was not clicked, but it has passed the charging stage, flag
    // the note as pending destruction.
    if (!(note_data->flags.note_pending_destruction)) {
        note_data->flags.note_pending_destruction = true;
        return false;
    }

    // If the note has passed its charging stage, is already flagged as pending,
    // and has passed the destruction-pending stage, destroy the note.
    if (note_data->current_frame >=
        note_data->charge_frames + DESTRUCTION_FRAMES) {
        SpriteManagerRemoveSprite(THIS);

        RegisterNoteHit(HitMiss);

        return true;
    }

    return false;
}
