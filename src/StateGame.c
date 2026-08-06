#include "Banks/SetAutoBank.h"
#include "Maps.h"
#include "Scroll.h"
#include "ZGBMain.h"

IMPORT_MAP(map_background);

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    Sprite *note1 = SpriteManagerAdd(SpriteTapNoteHorizontal, 16, 56);
    note1->mirror = V_MIRROR;

    SpriteManagerAdd(SpriteTapNoteVertical, 80, 56);
    SpriteManagerAdd(SpriteTapNoteVertical, 128, 56);

    SpriteManagerAdd(SpriteScanline, 70, 52);
}

void UPDATE(void) {}
