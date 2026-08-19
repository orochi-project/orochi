#include "Banks/SetAutoBank.h"

#include "GameData.h"
#include "GameStore.h"
#include "Keys.h"
#include "Maps.h"
#include "Notes.h"
#include "Print.h"
#include "Scanline.h"
#include "Scroll.h"
#include "Sound.h"
#include "SpriteManager.h"
#include "Text.h"
#include "ZGBMain.h"
#include <stdlib.h>
#include <string.h>

IMPORT_MAP(map_background);

IMPORT_TILES(mangrove_font_alternate);

#define HIT_GRADE_LABEL_DURATION 120;

/** Represents the stored font offsets for all loaded game fonts. */
typedef struct {
    int8_t mangrove_font_alternate_font_offset;
} FontOffset;

/** The saved game font offsets. */
static FontOffset font_offsets;

/** The currently loaded map. */
static const Map *current_map;

/** The current frame in the game. */
static uint16_t current_frame;

/** The index of the next note. */
static uint16_t next_note_idx;

/** The countdown to when the hit grade label should automatically clear. */
static uint8_t hit_grade_label_timer;

/** The last accuracy percentage drawn to the HUD. */
static uint8_t last_drawn_accuracy_percent;

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

void START(void) {
    DISABLE_SPRITE_FLICKERING; // ... otherwise it looks glitchy

    current_map = &maps[selected_map_idx];
    current_frame = 0;
    next_note_idx = 0;
    hit_grade_label_timer = 0;
    last_accuracy_percent = 100;
    notes_hit_count = 0;
    last_drawn_accuracy_percent = 255;

    ResetAccuracy();

    InitGameAudio(); // custom init

    InitScroll(BANK(map_background), &map_background, 0, 0);

    INIT_FONT(mangrove_font_alternate, PRINT_BKG);
    font_offsets.mangrove_font_alternate_font_offset = font_offset;

    SpriteManagerAdd(SpriteScanline, SCANLINE_START_X, SCANLINE_START_Y);

    ResetAllTypewriters();

    PlayCurrentMapSong();
}

void UPDATE(void) {
    ++current_frame;
    UpdateTypewriter();

    // NOTE: This might be too obscure/hidden for the average user to figure
    // out.
    // TODO: Try to make a small indicator for this.
    if (KEY_PRESSED(J_SELECT) && KEY_PRESSED(J_START))
        SetState(StateMenu);

    // End the song after the last frame.
    // TODO: Show the win screen.
    if (current_frame > current_map->frame_count) {
        sfx_sound_cut();
        SetState(StateEnd);
        return;
    }

    TickGameAudio(); // tick manually here because CrossZGB's audio ticking is
                     // faster than and independent of VBL updates

    while (next_note_idx < current_map->note_count) {
        Note current_note = GetMapNote(current_map->notes,
                                       current_map->notes_bank, next_note_idx);

        if (current_frame < current_note.appear_frame)
            break;

        DrawNote(&current_note);

        ++next_note_idx;
    }

    DrawHUD();
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
    font_offset = font_offsets.mangrove_font_alternate_font_offset;

    static unsigned char grade_filler_label[] = "       "; // 7 spaces
    static unsigned char accuracy_icon_label[] = "!";
    static unsigned char accuracy_filler_label[] = "    "; // 4 spaces

    if (should_draw_hit_grade_label) {
        DrawText(grade_filler_label, 19, 16, TEXT_ANCHOR_RIGHT, 0,
                 TEXT_ALTERNATE_PALETTE_IDX);

        static unsigned char grade_label[8];

        switch (latest_hit_grade) {
        case HitPerfect:
            strcpy((char *)grade_label, "PERFECT");
            ++notes_hit_count;
            break;
        case HitEarly:
            strcpy((char *)grade_label, "EARLY");
            ++notes_hit_count;
            break;
        case HitLate:
            strcpy((char *)grade_label, "LATE");
            ++notes_hit_count;
            break;
        case HitMiss:
        default:
            strcpy((char *)grade_label, "MISS");
            break;
        }

        DrawText((const unsigned char *)grade_label, 19, 16, TEXT_ANCHOR_RIGHT,
                 1, TEXT_ALTERNATE_PALETTE_IDX);

        should_draw_hit_grade_label = false;
        hit_grade_label_timer = HIT_GRADE_LABEL_DURATION;
    }

    if (hit_grade_label_timer > 0)
        if (--hit_grade_label_timer == 0)
            DrawText(grade_filler_label, 19, 16, TEXT_ANCHOR_RIGHT, 1,
                     TEXT_ALTERNATE_PALETTE_IDX);

    // draw the accuracy icon once on the first frame
    if (current_frame == 1)
        DrawText(accuracy_icon_label, 1, 16, TEXT_ANCHOR_LEFT, 0,
                 TEXT_ALTERNATE_PALETTE_IDX);

    if (last_accuracy_percent != last_drawn_accuracy_percent) {
        char accuracy_label[5];
        itoa(last_accuracy_percent, accuracy_label, 10);

        const uint8_t digit_count = strlen(accuracy_label);
        accuracy_label[digit_count] = '('; // % symbol
        accuracy_label[digit_count + 1] = '\0';

        DrawText(accuracy_filler_label, 2, 16, TEXT_ANCHOR_LEFT, 0,
                 TEXT_ALTERNATE_PALETTE_IDX);
        DrawText((const unsigned char *)accuracy_label, 2, 16, TEXT_ANCHOR_LEFT,
                 0, TEXT_ALTERNATE_PALETTE_IDX);

        last_drawn_accuracy_percent = last_accuracy_percent;
    }
}
