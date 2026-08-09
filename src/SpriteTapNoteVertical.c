#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "Notes.h"
#include "ZGBMain.h"

/** The total number of frames for this note. */
#define NOTE_FRAME_COUNT 5

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
    TapNoteData *note_data = (TapNoteData *)THIS->custom_data;
    note_data->current_frame = 0;
}

void UPDATE(void) {
    TapNoteData *note_data = (TapNoteData *)THIS->custom_data;

    ++note_data->current_frame;

    // charge period
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t note_frame =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_FRAME_COUNT - 1);
        SetFrame(THIS, note_frame);
    }

    // If the note collides with the scanline and the correct direction is
    // pressed, delete the note.
    // TODO: check for early hits
    if (CheckCollision(THIS, scanline_sprite)) {
        if ((THIS->mirror && KEY_TICKED(J_DOWN)) ||
            (!THIS->mirror && KEY_TICKED(J_UP))) {
            SpriteManagerRemoveSprite(THIS);
            return;
        }
    }

    // perfect period
    if (note_data->current_frame >=
        note_data->charge_frames + NOTE_PERFECT_FRAMES) {
        SpriteManagerRemoveSprite(THIS);
        return;
    }

    // TODO: check for misses
}

void DESTROY(void) {}
