#include "Notes.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include <stdint.h>

Sprite *DrawNote(Note note) {
    // get row/col
    uint8_t grid_row = note.grid_idx / GRID_COLS;
    uint8_t grid_col = note.grid_idx % GRID_COLS;

    // get xy
    uint8_t pixel_x = GRID_START_X + NOTE_WIDTH * grid_col;
    uint8_t pixel_y = GRID_START_Y + NOTE_HEIGHT * grid_row;

    uint8_t note_sprite_type;
    switch (note.type) {
    case TapLeft:
    case TapRight:
        note_sprite_type = SpriteTapNoteHorizontal;
        break;

    case TapUp:
    case TapDown:
        note_sprite_type = SpriteTapNoteVertical;
        break;

    default:
        return NULL; // TODO: implement other notes
    }

    Sprite *note_sprite = SpriteManagerAdd(note_sprite_type, pixel_x, pixel_y);
    if (!note_sprite)
        return NULL;

    // flip if needed
    if (note.type == TapLeft) {
        note_sprite->mirror = V_MIRROR;
        note_sprite->x -= 16;
    } else if (note.type == TapDown) {
        note_sprite->mirror = H_MIRROR;
        note_sprite->y -= 16;
    }

    return note_sprite;
}
