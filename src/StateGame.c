#include "Banks/SetAutoBank.h"
#include "Maps.h"
#include "Notes.h"
#include "Scroll.h"
#include "ZGBMain.h"

IMPORT_MAP(map_background);

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    note(SpriteTapNoteHorizontal, 16, 56, FALSE);
    note(SpriteTapNoteHorizontal, 48, 56, TRUE);
    note(SpriteTapNoteVertical, 80, 56, FALSE);
    note(SpriteTapNoteVertical, 128, 56, TRUE);

    SpriteManagerAdd(SpriteScanline, 70, 52);
}

void UPDATE(void) {}
