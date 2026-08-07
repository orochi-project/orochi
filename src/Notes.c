#include "Notes.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

// Displays a note on the screen
Sprite *note(UINT8 note_type, UINT16 x_pos, UINT16 y_pos, UINT8 flipped) {
    Sprite *spr = SpriteManagerAdd(note_type, x_pos, y_pos);
    if (!spr)
        return NULL;

    if (flipped) {
        if (note_type == SpriteTapNoteHorizontal) {
            spr->mirror = V_MIRROR;
            spr->x -= 16;
        } else if (note_type == SpriteTapNoteVertical) {
            spr->mirror = H_MIRROR;
            spr->y -= 16;
        }
    }

    return spr;
}