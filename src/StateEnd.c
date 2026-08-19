#include "Banks/SetAutoBank.h"

#include "GameData.h"
#include "GameStore.h"
#include "Keys.h"
#include "Maps.h"
#include "Music.h"
#include "Palette.h"
#include "Print.h"
#include "SpriteManager.h"
#include "Text.h"
#include "ZGBMain.h"
#include "gbdk/platform.h"
#include "stdlib.h"
#include <string.h>

IMPORT_MAP(end_background);
IMPORT_MAP(end_stats_box);

IMPORT_TILES(symbolic_font_primary);
IMPORT_TILES(yarara_font_primary);
IMPORT_TILES(mangrove_font_primary);

DECLARE_MUSIC(detour);

extern const palette_color_t end_stats_box_palettes[4];

extern void RefreshLetterGrade(Sprite *sprite) BANKED;

/** The minimum accuracy required for an SS grade. */
#define GRADE_SS_THRESHOLD 100
/** The minimum accuracy required for an S grade. */
#define GRADE_S_THRESHOLD 95
/** The minimum accuracy required for an A grade. */
#define GRADE_A_THRESHOLD 90
/** The minimum accuracy required for a B grade. */
#define GRADE_B_THRESHOLD 80
/** The minimum accuracy required for a C grade. */
#define GRADE_C_THRESHOLD 60
/** The minimum accuracy required for a D grade. */
#define GRADE_D_THRESHOLD 0

/** The starting x-position to place the stats box. */
#define STATS_BOX_TILE_X 3
/** The starting y-position to place the stats box. */
#define STATS_BOX_TILE_Y 5
/** The palette index that points to the stats box overlay colors. */
#define STATS_BOX_PALETTE_IDX 4

/** Represents the possible logo states. */
typedef enum {
    END_TITLE_SYMBOLIC_ICON,
    END_TITLE_TEXT_LABEL,
    END_STATS_BOX,
    END_MAP_NAME_LABEL,
    END_PLAYER_GRADE_SPRITE,
    END_STAT_LABELS,
} EndCheckpoint;

/** Represents the stored font offsets for all loaded end-screen fonts. */
typedef struct {
    int8_t symbolic_font_primary_font_offset;
    int8_t yarara_font_primary_font_offset;
    int8_t mangrove_font_primary_font_offset;
} FontOffset;

/** The current end-screen state. */
static EndCheckpoint end_checkpoint;

/** The saved game font offsets. */
static FontOffset font_offsets;

/** The currently selected map. */
static const Map *selected_map;

/** The title symbolic icon typewriter index. */
static int8_t title_symbolic_icon_typewriter_idx;
/** The title label typewriter index. */
static int8_t title_text_typewriter_idx;
/** The typewriter index for the map name text. */
static int8_t map_name_typewriter_idx;
/** The typewriter index for the final accuracy text. */
static int8_t final_accuracy_typewriter_idx;
/** The typewriter index for the note count text. */
static int8_t note_count_typewriter_idx;

/** Draw the end-screen title symbolic icon. */
static void DrawTitleSymbolicIcon(void);

/** Draw the end-screen title text label. */
static void DrawTitleTextLabel(void);

/** Draw the stats box overlay. */
static void DrawOverlayStatsBox(void) NONBANKED;

/** Draw the map name label. */
static void DrawMapNameLabel(void);

/** Draw the final letter grade sprite. */
static Sprite *DrawPlayerGradeSprite(void);

/** Draw the final stat labels. */
static void DrawStatLabels(void);

void START(void) {
    selected_map = &maps[selected_map_idx];
    end_checkpoint = END_TITLE_SYMBOLIC_ICON;

    RestoreDefaultAudio();

    InitScroll(BANK(end_background), &end_background, 0, 0);

    INIT_FONT(symbolic_font_primary, PRINT_BKG);
    font_offsets.symbolic_font_primary_font_offset = font_offset;

    INIT_FONT(yarara_font_primary, PRINT_BKG);
    font_offsets.yarara_font_primary_font_offset = font_offset;

    INIT_FONT(mangrove_font_primary, PRINT_BKG);
    font_offsets.mangrove_font_primary_font_offset = font_offset;

    ResetAllTypewriters();

    DrawTitleSymbolicIcon();
}

void UPDATE(void) {
    UpdateTypewriter();

    // title symbolic icon done
    // now draw title text label
    if (end_checkpoint == END_TITLE_SYMBOLIC_ICON &&
        TypewriterIsDone(title_symbolic_icon_typewriter_idx)) {
        DrawTitleTextLabel();
        end_checkpoint = END_TITLE_TEXT_LABEL;
    }

    // title text label done
    // now draw stats box
    if (end_checkpoint == END_TITLE_TEXT_LABEL &&
        TypewriterIsDone(title_text_typewriter_idx)) {
        DrawOverlayStatsBox();
        PlayMusic(detour, 1);
        end_checkpoint = END_STATS_BOX;
    }

    // stats box done
    // now draw map name label
    if (end_checkpoint == END_STATS_BOX) {
        DrawMapNameLabel();
        end_checkpoint = END_MAP_NAME_LABEL;
    }

    // map name label done
    // now draw player grade sprite
    if (end_checkpoint == END_MAP_NAME_LABEL &&
        TypewriterIsDone(map_name_typewriter_idx)) {
        DrawPlayerGradeSprite();
        end_checkpoint = END_PLAYER_GRADE_SPRITE;
    }

    // player grade sprite done
    // now stat labels
    if (end_checkpoint == END_PLAYER_GRADE_SPRITE) {
        DrawStatLabels();
        end_checkpoint = END_STAT_LABELS;
    }

    // stat labels done
    // now detect keypresses
    if (end_checkpoint == END_STAT_LABELS &&
        TypewriterIsDone(final_accuracy_typewriter_idx) &&
        TypewriterIsDone(note_count_typewriter_idx) && KEY_TICKED(J_START))
        SetState(StateMenu);
}

static void DrawTitleSymbolicIcon(void) {
    wait_vbl_done();

    font_offset = font_offsets.symbolic_font_primary_font_offset;

    static unsigned char title_symbolic_icon_upper[] = "IJ"; // graph icon
    static unsigned char title_symbolic_icon_lower[] = "KL"; // graph icon

    DrawText(title_symbolic_icon_upper, 3, 1, TEXT_ANCHOR_LEFT, 10,
             TEXT_PRIMARY_PALETTE_IDX);
    DrawText(title_symbolic_icon_lower, 3, 2, TEXT_ANCHOR_LEFT, 10,
             TEXT_PRIMARY_PALETTE_IDX);
}

static void DrawTitleTextLabel(void) {
    wait_vbl_done();

    font_offset = font_offsets.yarara_font_primary_font_offset;

    static unsigned char title_text_label[] = "STATISTICS!";

    DrawText(title_text_label, 6, 2, TEXT_ANCHOR_LEFT, 10,
             TEXT_PRIMARY_PALETTE_IDX);
}

static void DrawOverlayStatsBox(void) NONBANKED {
    wait_vbl_done();

    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(BANK(end_stats_box));

    // set palette for stats box manually
    SetPalette(BG_PALETTE, STATS_BOX_PALETTE_IDX, 1, end_stats_box_palettes,
               BANK(end_stats_box));

    // load the tile graphics
    // LoadMap() basically does this, but we need to do this manually to be
    // able to patch the attributes and thus set the palette correctly
    uint16_t map_offset = ScrollSetTiles(
        last_tile_loaded, end_stats_box.tiles_bank, end_stats_box.tiles);

    // patch each tile's attribute byte to point at our slot instead of 0,
    // since the exported asset hardcodes palette index 0 for whatever
    // reason
    const unsigned char *data = end_stats_box.data;
    const unsigned char *attributes = end_stats_box.attributes;

    for (uint8_t y = 0; y < end_stats_box.height; ++y)
        for (uint8_t x = 0; x < end_stats_box.width; ++x) {
            uint8_t patched_attributes =
                (*attributes & ~0x07) | STATS_BOX_PALETTE_IDX;

            UpdateMapTile(TARGET_BKG, STATS_BOX_TILE_X + x,
                          STATS_BOX_TILE_Y + y, map_offset, *data,
                          &patched_attributes);

            ++data;
            ++attributes;
        }

    SWITCH_ROM(_saved_bank);
}

static void DrawMapNameLabel(void) {
    wait_vbl_done();

    font_offset = font_offsets.yarara_font_primary_font_offset;

    static unsigned char map_title[MAP_NAME_MAX_LENGTH];
    memset(map_title, ' ',
           MAP_NAME_MAX_LENGTH - 1); // subtract 1 because of '\0'
    map_title[MAP_NAME_MAX_LENGTH - 1] = '\0';
    strncpy((char *)map_title, selected_map->name, strlen(selected_map->name));

    map_name_typewriter_idx = DrawText(map_title, 4, 6, TEXT_ANCHOR_LEFT, 3,
                                       TEXT_PRIMARY_PALETTE_IDX);
}

static Sprite *DrawPlayerGradeSprite(void) {
    wait_vbl_done();

    Sprite *letter_grade_sprite = SpriteManagerAdd(SpriteLetterGrade, 32, 60);
    if (!letter_grade_sprite)
        return NULL;

    LetterGradeData *letter_grade_data =
        (LetterGradeData *)letter_grade_sprite->custom_data;

    if (last_accuracy_percent >= GRADE_SS_THRESHOLD)
        letter_grade_data->letter = 'M';
    else if (last_accuracy_percent >= GRADE_S_THRESHOLD)
        letter_grade_data->letter = 'S';
    else if (last_accuracy_percent >= GRADE_A_THRESHOLD)
        letter_grade_data->letter = 'A';
    else if (last_accuracy_percent >= GRADE_B_THRESHOLD)
        letter_grade_data->letter = 'B';
    else if (last_accuracy_percent >= GRADE_C_THRESHOLD)
        letter_grade_data->letter = 'C';
    else if (last_accuracy_percent >= GRADE_D_THRESHOLD)
        letter_grade_data->letter = 'D';

    RefreshLetterGrade(letter_grade_sprite);

    return letter_grade_sprite;
}

static void DrawStatLabels(void) {
    wait_vbl_done();

    font_offset = font_offsets.mangrove_font_primary_font_offset;

    static char final_accuracy_label[6] = "!"; // accuracy symbol
    itoa(last_accuracy_percent, final_accuracy_label + 1,
         10); // accuracy
              // +1 to insert after acc. symbol
    const uint8_t final_accuracy_text_length = strlen(final_accuracy_label);
    final_accuracy_label[final_accuracy_text_length] = '('; // %
    final_accuracy_label[final_accuracy_text_length + 1] = '\0';

    final_accuracy_typewriter_idx =
        DrawText((const unsigned char *)final_accuracy_label, 7, 8,
                 TEXT_ANCHOR_LEFT, 3, TEXT_PRIMARY_PALETTE_IDX);

    static char note_count_label[7] = ")";           // note symbol
    itoa(notes_hit_count, note_count_label + 1, 10); // note count
    const uint8_t note_count_text_length = strlen(note_count_label);
    note_count_label[note_count_text_length] = '-';
    note_count_label[note_count_text_length + 1] = '\0';

    note_count_typewriter_idx =
        DrawText((const unsigned char *)note_count_label, 7, 9,
                 TEXT_ANCHOR_LEFT, 3, TEXT_PRIMARY_PALETTE_IDX);
}
