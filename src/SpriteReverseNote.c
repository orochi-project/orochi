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
    note_data->current_frame = 0;
    note_data->flags = 0x00;
}

void UPDATE(void) {
    ReverseNoteData *note_data = (ReverseNoteData *)THIS->custom_data;
    ScanlineData *scanline_data = (ScanlineData *)scanline_sprite->custom_data;

    ++note_data->current_frame;

    // charge period
    if (note_data->current_frame <= note_data->charge_frames) {
        uint16_t note_frame =
            DIV_MUL_ROUND(note_data->current_frame, note_data->charge_frames,
                          NOTE_FRAME_COUNT - 1);
        SetFrame(THIS, note_frame);
    }

    // snap scanline
    if (!(note_data->flags & FLAG_SNAPPED) &&
        note_data->current_frame >=
            note_data->charge_frames) { // if not snapped and charge done
        // snap x
        scanline_sprite->x = SCANLINE_BOUND_LEFT_X + note_data->scanline_x;

        // snap direction
        if ((scanline_data->velocity < 0) !=
            (note_data->scanline_direction < 0))
            scanline_data->velocity = -scanline_data->velocity;

        // flag as snapped
        note_data->flags |= FLAG_SNAPPED;
    }

    // reverse scanline
    if (!(note_data->flags & FLAG_REVERSED) &&
        note_data->current_frame >=
            note_data->charge_frames) { // if not reversed and charge done
        scanline_data->velocity = -scanline_data->velocity;

        // flag as reversed
        note_data->flags |= FLAG_REVERSED;

        if (note_data->flags &
            FLAG_CLICKED) { // if flagged as clicked already, then remove sprite
                            // once reversed
            SpriteManagerRemoveSprite(THIS);
            return;
        }
    }

    // speed modifier
    if (!(note_data->flags & FLAG_SPEED_CHANGED) && note_data->speed_modifier &&
        note_data->current_frame >=
            note_data->charge_frames) { // if not speed changed and charge done
        // change velocity
        scanline_data->velocity =
            SIGN(scanline_data->velocity) * note_data->speed_modifier;
        // flag as speed changed
        note_data->flags |= FLAG_SPEED_CHANGED;
    }

    // If the note collides with the scanline and the correct direction is
    // pressed, delete the note.
    // TODO: check for early hits
    if (!(note_data->flags & FLAG_CLICKED) &&
        CheckCollision(THIS, scanline_sprite) &&
        KEY_TICKED(J_B)) { // if not clicked and B is ticked
        // flag as clicked
        // don't delete the sprite yet; otherwise it might look odd for the
        // scanline to reverse on an empty sprite
        note_data->flags |= FLAG_CLICKED;
        return;
    }

    // perfect period
    if (note_data->current_frame >= note_data->charge_frames) {
        SpriteManagerRemoveSprite(THIS);
        return;
    }

    // TODO: check for misses
}

void DESTROY(void) {}
