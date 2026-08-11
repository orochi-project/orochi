#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "MathUtils.h"
#include "Notes.h"
#include "Scanline.h"
#include "SpriteManager.h"
#include <stdbool.h>

/** The total number of charge frames for this note. */
#define NOTE_CHARGE_FRAME_COUNT 5

/** The total number of hold frames for this note. */
#define NOTE_HOLD_FRAME_COUNT 4

/**
 * Update the charge and hold animation frames of the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void UpdateChargeAnimation(HoldNoteData *note_data);

/**
 * Check if the player hit the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void CheckPlayerClick(HoldNoteData *note_data);

/**
 * Apply scanline modifiers for the note.
 *
 * @param note_data     A pointer to the note custom data.
 * @param scanline_data A pointer to the scanline custom data.
 */
static void ApplyScanlineModifiers(HoldNoteData *note_data,
                                   ScanlineData *scanline_data);

/**
 * Handle the note's destruction.
 *
 * @param note_data A pointer to the note custom data.
 *
 * @return Whether or not the sprite was destroyed.
 */
static bool HandleDestruction(HoldNoteData *note_data);

void START(void) {
    HoldNoteData *note_data = (HoldNoteData *)THIS->custom_data;
    note_data->current_frame = 0;
    note_data->flags = 0x00;
}

void UPDATE(void) {
    HoldNoteData *note_data = (HoldNoteData *)THIS->custom_data;
    ScanlineData *scanline_data = (ScanlineData *)scanline_sprite->custom_data;

    ++note_data->current_frame;

    UpdateChargeAnimation(note_data);
    CheckPlayerClick(note_data);
    ApplyScanlineModifiers(note_data, scanline_data);

    if (HandleDestruction(note_data))
        return;
}

void DESTROY(void) {}

static void UpdateChargeAnimation(HoldNoteData *note_data) {
    // If the note is still in the charging stage, calculate the note frame
    // index and set the note animation frame to that index.
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t frame_idx =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_CHARGE_FRAME_COUNT - 1);
        SetFrame(THIS, frame_idx);
    }
    // If the note is in the holding stage, calculate the hold frame index and
    // set the note animation frame to that index.
    // TODO: Only show the holding animation if the user is actually holding the
    // note.
    else if (note_data->current_frame <=
             note_data->charge_frames + note_data->hold_frames) {
        uint16_t frame_idx =
            DIV_MUL_ROUND(note_data->current_frame - note_data->charge_frames,
                          note_data->hold_frames, NOTE_HOLD_FRAME_COUNT) +
            NOTE_CHARGE_FRAME_COUNT - 1;
        SetFrame(THIS, frame_idx);
    }
}

static void CheckPlayerClick(HoldNoteData *note_data) {
    // If the note was not already clicked and was just ticked this frame, move
    // the note off-screen and flag the note as clicked to prevent any later
    // clicks on this note from registering.
    if (!(note_data->flags & FLAG_CLICKED) &&
        CheckCollision(THIS, scanline_sprite) && KEY_TICKED(J_A)) {
        note_data->flags |= FLAG_CLICKED;
        // NOTE: This is temporary. There is no hold logic yet. This only
        // detects if the note was ticked.
        // TODO: Handle hold release.
    }
}

static void ApplyScanlineModifiers(HoldNoteData *note_data,
                                   ScanlineData *scanline_data) {
    // The scanline should only ever be modified after the note has passed its
    // charging stage.
    if (note_data->current_frame < note_data->charge_frames)
        return;

    // If the scanline was not already snapped to the note's assigned scanline
    // x-position and direction, snap it now.
    if (!(note_data->flags & FLAG_SNAPPED)) {
        scanline_sprite->x = SCANLINE_BOUND_LEFT_X + note_data->scanline_x;
        if ((scanline_data->velocity < 0) !=
            (note_data->scanline_direction < 0))
            scanline_data->velocity = -scanline_data->velocity;
        note_data->flags |= FLAG_SNAPPED;
    }

    // Freeze the scanline while the hold note is active.
    if (!scanline_data->frozen)
        scanline_data->frozen = true;

    // Unfreeze the scanline once the holding stage ends.
    if (note_data->current_frame >=
        note_data->charge_frames + note_data->hold_frames) {
        scanline_data->frozen = false;
    }

    // If we did not already change the velocity of the scanline to the note's
    // speed modifier, change it now.
    // If the note's speed modifier is 0 (or anything falsy), do not modify the
    // scanline's velocity. The scanline's direction stays the same.
    if (!(note_data->flags & FLAG_SPEED_CHANGED) && note_data->speed_modifier) {
        scanline_data->velocity =
            SIGN(scanline_data->velocity) * note_data->speed_modifier;
        note_data->flags |= FLAG_SPEED_CHANGED;
    }
}

static bool HandleDestruction(HoldNoteData *note_data) {
    // If the note has passed the hold period, destroy it.
    if (note_data->current_frame >
        note_data->charge_frames + note_data->hold_frames) {
        SpriteManagerRemoveSprite(THIS);
        return true;
    }

    // TODO: Check if the user held the note.

    return false;
}
