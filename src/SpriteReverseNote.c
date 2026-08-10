#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "Notes.h"
#include "Scanline.h"
#include "SpriteManager.h"
#include <stdbool.h>

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

/**
 * @def SIGN(x)
 *
 * Get the sign of a number.
 *
 * @param x The number to get the sign of.
 */
#define SIGN(x) (((x) > 0) - ((x) < 0))

void START(void) {
    ReverseNoteData *note_data = (ReverseNoteData *)THIS->custom_data;
    note_data->speed_modifier = 0;
    note_data->charge_frames = 0;
    note_data->current_frame = 0;
    note_data->speed_changed = false;
    note_data->reversed = false;
    note_data->clicked = false;
}

void UPDATE(void) {
    ReverseNoteData *note_data = (ReverseNoteData *)THIS->custom_data;

    ++note_data->current_frame;

    // charge period
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t note_frame =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_FRAME_COUNT - 1);
        SetFrame(THIS, note_frame);
    }

    if (!note_data->reversed &&
        (note_data->current_frame >= note_data->charge_frames)) {
        ScanlineData *scanline_data =
            (ScanlineData *)scanline_sprite->custom_data;
        scanline_data->velocity = -scanline_data->velocity;
        note_data->reversed = true;
        if (note_data->clicked) {
            SpriteManagerRemoveSprite(THIS);
            return;
        }
    }

    if (note_data->current_frame >= note_data->charge_frames &&
        !note_data->speed_changed && note_data->speed_modifier) {
        ScanlineData *scanline_data =
            (ScanlineData *)scanline_sprite->custom_data;
        scanline_data->velocity =
            SIGN(scanline_data->velocity) * note_data->speed_modifier;
        note_data->speed_changed = true;
    }

    // If the note collides with the scanline and the correct direction is
    // pressed, delete the note.
    // TODO: check for early hits
    if (!note_data->clicked && CheckCollision(THIS, scanline_sprite) &&
        KEY_TICKED(J_B)) {
        note_data->clicked = true;
        return;
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
