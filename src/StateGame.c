#include "Banks/SetAutoBank.h"
#include "GameData.h"
#include "MapDreamFlower.h"
#include "Notes.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include "gbdk/platform.h"

IMPORT_MAP(map_background);

static uint16_t current_frame = 0;
static uint16_t next_note_idx = 0;

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    SpriteManagerAdd(SpriteScanline, 70, 64);
}

void UPDATE(void) {
    ++current_frame;

    uint16_t note_count = MapDreamFlowerGetNoteCount();

    while (next_note_idx < note_count &&
           current_frame >= MapDreamFlowerGetNote(next_note_idx).appear_frame) {
        DrawNote(MapDreamFlowerGetNote(next_note_idx));
        ++next_note_idx;
    }
}
