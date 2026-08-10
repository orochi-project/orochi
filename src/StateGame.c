#include "Banks/SetAutoBank.h"
#include "GameData.h"
#include "MapDreamFlower.h"
#include "Notes.h"
#include "Scanline.h"
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
static Sprite *DrawNote(const Note *note);

void START(void) {
    InitScroll(BANK(map_background), &map_background, 0, 0);
    SpriteManagerAdd(SpriteScanline, SCANLINE_START_X, SCANLINE_START_Y);
    PlayCurrentMapSong();
}

void UPDATE(void) {
    ++current_frame;

    while (next_note_idx < MapDreamFlowerGetNoteCount()) {
        Note current_note = MapDreamFlowerGetNote(next_note_idx);
        if (current_frame < current_note.appear_frame)
            break;

        DrawNote(&current_note);
        ++next_note_idx;
    }
}

static Sprite *DrawNote(const Note *note) {
    // get row/col
    uint8_t grid_row = note->grid_idx / GRID_COLS;
    uint8_t grid_col = note->grid_idx % GRID_COLS;

    // get xy
    uint8_t pixel_x = GRID_START_X + NOTE_WIDTH * grid_col;
    uint8_t pixel_y = GRID_START_Y + NOTE_HEIGHT * grid_row;

    uint8_t note_sprite_type;
    switch (note->type) {
    case TapLeft:
    case TapRight:
        note_sprite_type = SpriteTapNoteHorizontal;
        break;
    case TapUp:
    case TapDown:
        note_sprite_type = SpriteTapNoteVertical;
        break;
    case Reverse:
        note_sprite_type = SpriteReverseNote;
        break;
    case Hold:
        note_sprite_type = SpriteHoldNote;
        break;
    default:
        return NULL; // TODO: implement other notes
    }

    Sprite *note_sprite = SpriteManagerAdd(note_sprite_type, pixel_x, pixel_y);
    if (!note_sprite)
        return NULL;

    // flip if needed
    if (note->type == TapLeft)
        note_sprite->mirror = V_MIRROR;
    else if (note->type == TapDown)
        note_sprite->mirror = H_MIRROR;

    // set charge frames
    switch (note->type) {
    case TapLeft:
    case TapRight:
    case TapUp:
    case TapDown: {
        TapNoteData *note_data = (TapNoteData *)note_sprite->custom_data;
        note_data->speed_modifier = note->speed_modifier;
        note_data->charge_frames = note->charge_frames;
        break;
    }
    case Reverse: {
        ReverseNoteData *note_data =
            (ReverseNoteData *)note_sprite->custom_data;
        note_data->speed_modifier = note->speed_modifier;
        note_data->charge_frames = note->charge_frames;
        break;
    }
    case Hold: {
        HoldNoteData *note_data = (HoldNoteData *)note_sprite->custom_data;
        note_data->speed_modifier = note->speed_modifier;
        note_data->charge_frames = note->charge_frames;
        note_data->hold_frames = note->hold_frames;
        break;
    }
    }

    return note_sprite;
}
