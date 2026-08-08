#include "Banks/SetAutoBank.h"
#include "Notes.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

IMPORT_MAP(map_background);

// dummy notes
static const Note notes[] = {
    {.type = TapLeft,
     .grid_idx = 18,
     .speed_modifier = 0,
     .appear_frame = 148,
     .charge_frames = 90,
     .hold_frames = 0},
    {.type = TapRight,
     .grid_idx = 25,
     .speed_modifier = 0,
     .appear_frame = 292,
     .charge_frames = 90,
     .hold_frames = 0},
};
#define NOTE_COUNT (sizeof(notes) / sizeof(notes[0]))

static uint16_t current_frame = 0;
static uint16_t next_note_idx = 0;

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    SpriteManagerAdd(SpriteScanline, 70, 64);
}

void UPDATE(void) {
    ++current_frame;

    while (next_note_idx < NOTE_COUNT &&
           current_frame >= notes[next_note_idx].appear_frame) {
        DrawNote(notes[next_note_idx]);
        ++next_note_idx;
    }
}
