#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "Notes.h"
#include "Scanline.h"
#include "SpriteManager.h"
#include <stdbool.h>

/** The total number of charge frames for this note. */
#define NOTE_CHARGE_FRAME_COUNT 5
/** The total number of hold frames for this note. */
#define NOTE_HOLD_FRAME_COUNT 4

/**
 * The number of frames after the charge period during which the player
 * can register a "perfect" click.
 */
#define NOTE_PERFECT_FRAMES 8

/**
 * @def DIV_MUL_ROUND(a, b, c)
 *
 * Divide a value after multiplying it, rounding to the nearest integer.
 *
 * @param a The value to divide.
 * @param b The divisor.
 * @param c The multiplier.
 */
#define DIV_MUL_ROUND(a, b, c) (((a) * (c) + (b) / 2) / (b))

void START(void) {
    HoldNoteData *note_data = (HoldNoteData *)THIS->custom_data;
    note_data->speed_modifier = 0;
    note_data->charge_frames = 0;
    note_data->current_frame = 0;
    note_data->hold_frames = 0;
    note_data->speed_changed = false;
}

void UPDATE(void) {
    HoldNoteData *note_data = (HoldNoteData *)THIS->custom_data;
    ScanlineData *scanline_data = (ScanlineData *)scanline_sprite->custom_data;

    ++note_data->current_frame;

    // charge period
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t note_frame =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_CHARGE_FRAME_COUNT - 1);
        SetFrame(THIS, note_frame);
    } else if (note_data->current_frame <=
               note_data->charge_frames + note_data->hold_frames) {
        uint16_t note_frame =
            DIV_MUL_ROUND(note_data->current_frame,
                          note_data->charge_frames + note_data->hold_frames,
                          NOTE_HOLD_FRAME_COUNT - 1) +
            NOTE_CHARGE_FRAME_COUNT - 1;
        SetFrame(THIS, note_frame);
    }

    if ((note_data->current_frame >= note_data->charge_frames) &&
        !scanline_data->frozen) {
        scanline_data->frozen = true;
    }

    if (scanline_data->frozen &&
        note_data->current_frame >=
            note_data->charge_frames + note_data->hold_frames) {
        scanline_data->frozen = false;
    }

    // If the note collides with the scanline and the correct direction is
    // pressed, delete the note.
    // TODO: check for early hits
    if (CheckCollision(THIS, scanline_sprite) && KEY_TICKED(J_A)) {
        SpriteManagerRemoveSprite(THIS);
        return;
    }

    // perfect period
    if (note_data->current_frame >= note_data->charge_frames +
                                        note_data->hold_frames +
                                        NOTE_PERFECT_FRAMES) {
        SpriteManagerRemoveSprite(THIS);
        return;
    }

    // TODO: check for misses
}

void DESTROY(void) {}
