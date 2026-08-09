#include "Banks/SetAutoBank.h"
#include "GameData.h"
#include "MapDreamFlower.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include "gbdk/platform.h"

IMPORT_MAP(map_background);

/** The current frame in the game. */
static uint16_t current_frame = 0;
/** The index of the next note. */
static uint16_t next_note_idx = 0;

/**
 * Display a note on the screen using Sprite Manager
 *
 * @param note  The note to display.
 *
 * @return A pointer to the displayed sprite.
 */
static Sprite *DrawNote(Note note);

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    SpriteManagerAdd(SpriteScanline, 8, 64);
    PlayCurrentMapSong();
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

static Sprite *DrawNote(Note note) {
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

    // set charge frames
    TapNoteData *note_data = (TapNoteData *)note_sprite->custom_data;
    note_data->charge_frames = note.charge_frames;

    return note_sprite;
}
