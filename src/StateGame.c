#include "Banks/SetAutoBank.h"
#include "GameAudio.h"
#include "GameData.h"
#include "Keys.h"
#include "MapDreamFlower.h"
#include "Notes.h"
#include "Print.h"
#include "Scanline.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Text.h"
#include "ZGBMain.h"

IMPORT_MAP(map_background);

IMPORT_TILES(mangrove_font_utility);

/** Represents the stored font offsets for all loaded game fonts. */
typedef struct {
    int8_t mangrove_font_utility_font_offset;
} FontOffset;

/** The saved game font offsets. */
static FontOffset font_offsets;

/** The current frame in the game. */
static uint16_t current_frame;

/** The index of the next note. */
static uint16_t next_note_idx;

/**
 * Display a note on the screen using Sprite Manager.
 *
 * @param note  The note to display.
 *
 * @return A pointer to the displayed sprite.
 */
static Sprite *DrawNote(const Note *note);

/**
 * Draw the performance indicator, healthbar, and health points.
 */
static void DrawHUD(void);

/**
 * Converts a string that contains numbers to characters that can be read and
 * displayed as healh bars.
 *
 * @param player_health Current player health as a string.
 *
 */
static char *HealthBar(char *player_health);

void START(void) {
    DISABLE_SPRITE_FLICKERING; // ... otherwise it looks glitchy

    current_frame = 0;
    next_note_idx = 0;

    InitGameAudio(); // custom init

    InitScroll(BANK(map_background), &map_background, 0, 0);

    INIT_FONT(mangrove_font_utility, PRINT_BKG);
    font_offsets.mangrove_font_utility_font_offset = font_offset;

    SpriteManagerAdd(SpriteScanline, SCANLINE_START_X, SCANLINE_START_Y);

    PlayCurrentMapSong();
}

void UPDATE(void) {
    TickGameAudio(); // tick manually here because CrossZGB's audio ticking is
                     // faster than and independent of VBL updates
    ++current_frame;

    while (next_note_idx < MapDreamFlowerGetNoteCount()) {
        Note current_note = MapDreamFlowerGetNote(next_note_idx);

        if (current_frame < current_note.appear_frame)
            break;

        DrawNote(&current_note);

        ++next_note_idx;
    }

    DrawHUD();
    // NOTE: This might be too obscure/hidden for the average user to figure
    // out.
    // TODO: Try to make a small indicator for this.
    if (KEY_PRESSED(J_SELECT) && KEY_PRESSED(J_START))
        SetState(StateMenu);
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
        note_data->scanline_x = note->scanline_x;
        note_data->scanline_direction = note->scanline_direction;
        break;
    }
    case Reverse: {
        ReverseNoteData *note_data =
            (ReverseNoteData *)note_sprite->custom_data;
        note_data->speed_modifier = note->speed_modifier;
        note_data->charge_frames = note->charge_frames;
        note_data->scanline_x = note->scanline_x;
        note_data->scanline_direction = note->scanline_direction;
        break;
    }
    case Hold: {
        HoldNoteData *note_data = (HoldNoteData *)note_sprite->custom_data;
        note_data->speed_modifier = note->speed_modifier;
        note_data->charge_frames = note->charge_frames;
        note_data->hold_frames = note->hold_frames;
        note_data->scanline_x = note->scanline_x;
        note_data->scanline_direction = note->scanline_direction;
        break;
    }
    }

    return note_sprite;
}

static void DrawHUD(void) {
    wait_vbl_done();

    font_offset = font_offsets.mangrove_font_utility_font_offset;

    DrawText("PERFECT", 1, 16, TEXT_ANCHOR_LEFT, 0, TEXT_UTILITY_PALETTE_IDX);
    DrawText(HealthBar("66662"), 9, 16, TEXT_ANCHOR_LEFT, 0,
             TEXT_UTILITY_PALETTE_IDX);
    DrawText("1000", 15, 16, TEXT_ANCHOR_LEFT, 0, TEXT_UTILITY_PALETTE_IDX);
}

static char *HealthBar(char *player_health) {

    static char translated_string[6] = "     ";
    char translator[] = "!'()-.:";
    for (uint8_t i = 0; i < strlen(player_health); ++i) {
        translated_string[i] =
            translator[player_health[i] - '0']; // Converts string value to
                                                // literal numerical value.
    }
    return translated_string;
}