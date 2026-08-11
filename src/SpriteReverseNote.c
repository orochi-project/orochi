#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "MathUtils.h"
#include "Notes.h"
#include "Scanline.h"
#include "SpriteManager.h"
#include <stdbool.h>

/** The total number of frames for this note. */
#define NOTE_FRAME_COUNT 5

/** The number of frames to allocate for the destruction timer. */
#define DESTRUCTION_FRAMES 30

/**
 * Update the charge animation frame of the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void UpdateChargeAnimation(ReverseNoteData *note_data);

/**
 * Check if the player hit the note.
 *
 * @param note_data A pointer to the note custom data.
 */
static void CheckPlayerClick(ReverseNoteData *note_data);

/**
 * Apply scanline modifiers for the note.
 *
 * @param note_data     A pointer to the note custom data.
 * @param scanline_data A pointer to the scanline custom data.
 */
static void ApplyScanlineModifiers(ReverseNoteData *note_data,
                                   ScanlineData *scanline_data);

/**
 * Handle the note's destruction.
 *
 * @param note_data A pointer to the note custom data.
 *
 * @return Whether or not the sprite was destroyed.
 */
static bool HandleDestruction(ReverseNoteData *note_data);

void START(void) {
    ReverseNoteData *note_data = (ReverseNoteData *)THIS->custom_data;
    note_data->current_frame = 0;
    note_data->flags = 0x00;
}

void UPDATE(void) {
    ReverseNoteData *note_data = (ReverseNoteData *)THIS->custom_data;
    ScanlineData *scanline_data = (ScanlineData *)scanline_sprite->custom_data;

    ++note_data->current_frame;

    UpdateChargeAnimation(note_data);
    CheckPlayerClick(note_data);
    ApplyScanlineModifiers(note_data, scanline_data);

    if (HandleDestruction(note_data))
        return;
}

void DESTROY(void) {}

static void UpdateChargeAnimation(ReverseNoteData *note_data) {
    // If the note is still in the charging stage, calculate the note frame
    // index and set the note animation frame to that index.
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t frame_idx =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_FRAME_COUNT - 1);
        SetFrame(THIS, frame_idx);
    }
}

static void CheckPlayerClick(ReverseNoteData *note_data) {
    // If the note was not already clicked and was just ticked this frame, move
    // the note off-screen and flag the note as clicked to prevent any later
    // clicks on this note from registering.
    if (!(note_data->flags & FLAG_CLICKED) &&
        CheckCollision(THIS, scanline_sprite) && KEY_TICKED(J_B)) {
        THIS->y = 144; // move below visible screen
        note_data->flags |= FLAG_CLICKED;
    }
}

static void ApplyScanlineModifiers(ReverseNoteData *note_data,
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

    // If we did not reverse the direction of the scanline before, reverse it
    // now.
    if (!(note_data->flags & FLAG_REVERSED)) {
        scanline_data->velocity = -scanline_data->velocity;
        note_data->flags |= FLAG_REVERSED;
    }

    // If we did not already change the velocity of the scanline to the note's
    // speed modifier, change it now.
    // If the note's speed modifier is 0 (or anything falsy), do not modify the
    // scanline's velocity.
    // The scanline's direction stays the same.
    if (!(note_data->flags & FLAG_SPEED_CHANGED) && note_data->speed_modifier) {
        scanline_data->velocity =
            SIGN(scanline_data->velocity) * note_data->speed_modifier;
        note_data->flags |= FLAG_SPEED_CHANGED;
    }
}

static bool HandleDestruction(ReverseNoteData *note_data) {
    if (note_data->current_frame < note_data->charge_frames)
        return false;

    // If the note was clicked, destroy the note.
    // TODO: Handle early clicks and misses.
    if (note_data->flags & FLAG_CLICKED) {
        SpriteManagerRemoveSprite(THIS);
        return true;
    }

    // If the note was not clicked, but it has passed the charging stage, flag
    // the note as pending destruction.
    if (!(note_data->flags & FLAG_DESTROY_PENDING))
        note_data->flags |= FLAG_DESTROY_PENDING;
    // If the note has passed its charging stage, is already flagged as pending,
    // and has passed the destruction-pending stage, destroy the note.
    else if (note_data->current_frame >=
             note_data->charge_frames + DESTRUCTION_FRAMES) {
        SpriteManagerRemoveSprite(THIS);
        return true;
    }

    return false;
}
