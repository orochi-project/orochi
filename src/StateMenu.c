#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "Maps.h"
#include "Print.h"
#include "Scroll.h"
#include "Text.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include <stdint.h>
#include <string.h>

IMPORT_MAP(menu_map);

IMPORT_TILES(japanese_glyphs);
IMPORT_TILES(yarara_font_primary);
IMPORT_TILES(yarara_font_secondary);
IMPORT_TILES(map_selector_tiles);

/** The starting x-position to place the map selector. */
#define MAP_SELECTOR_TILE_X 3
/** The starting y-position to place the map selector. */
#define MAP_SELECTOR_TILE_Y 5
/** The number of map selector rows to display. */
#define MAP_SELECTOR_ROW_COUNT 11
/** The palette index that points to the map selector tile colors. */
#define MAP_SELECTOR_PALETTE_IDX 3

/** Represents the possible logo states. */
typedef enum {
    MENU_LOGO_KANJI,
    MENU_LOGO_ROMAJI,
    MENU_OVERLAY_MAP_SELECTOR,
    MENU_MAP_LABELS,
} MenuCheckpoint;

/** Represents the stored font offsets for all loaded menu fonts. */
typedef struct {
    int8_t japanese_glyphs_font_offset;
    int8_t yarara_font_primary_font_offset;
    int8_t yarara_font_secondary_font_offset;
    int8_t map_selector_tiles_font_offset;
} FontOffset;

/** The saved menu font offsets. */
static FontOffset font_offsets;

/** The current menu state. */
static MenuCheckpoint menu_checkpoint;
/** The upper kanji typewriter index. */
static int8_t kanji_upper_typewriter_idx;
/** The lower kanji typewriter index. */
static int8_t kanji_lower_typewriter_idx;
/** The romaji typewriter index. */
static int8_t romaji_typewriter_idx;

/** The index of the currently selected map. */
static uint8_t selected_map_idx = 0;

/** The typewriter index for the map ID text. */
static int8_t map_id_typewriter_idx = -1;
/** The typewriter index for the map difficulty text. */
static int8_t map_difficulty_typewriter_idx = -1;
/** The typewriter index for the map name text. */
static int8_t map_name_typewriter_idx = -1;

/** The map selector row tiles as font characters. */
const char *map_selector_rows[MAP_SELECTOR_ROW_COUNT] = {
    "AEEEEEEEEEEEEC", "GIIIIIIIIIIIIH", "GIIIIIIIIIIIIH", "GIIIIIIIIIIIIH",
    "JJJJJJJJJJJJJJ", "JJJJJJJJJJJJJJ", "JJJJJJJJJJJJJJ", "GKLMNIIIISTUVH",
    "GOPQRIIIIWXYZH", "GII01234567IIH", "BFFFFFFFFFFFFD",
}; // yes, this is rendered as a font because I couldn't figure out how to
   // insert an image onto an existing background. sorry!

/** Draw the logo kanji text. */
void DrawLogoKanji(void);

/** Draw the logo romaji text. */
void DrawLogoRomaji(void);

void DrawMapLabels(void);
/**
 * Draw the map selector overlay.
 *
 * @param tile_x    The starting x-tile to draw the overlay.
 * @param tile_y    The starting y-tile to draw the overlay.
 */
void DrawOverlayMapSelector(uint8_t tile_x, uint8_t tile_y);

void START(void) {
    // scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
    InitScroll(BANK(menu_map), &menu_map, 0, 0);

    INIT_FONT(japanese_glyphs, PRINT_BKG);
    font_offsets.japanese_glyphs_font_offset = font_offset;

    INIT_FONT(yarara_font_primary, PRINT_BKG);
    font_offsets.yarara_font_primary_font_offset = font_offset;

    INIT_FONT(yarara_font_secondary, PRINT_BKG);
    font_offsets.yarara_font_secondary_font_offset = font_offset;

    INIT_FONT(map_selector_tiles, PRINT_BKG);
    font_offsets.map_selector_tiles_font_offset = font_offset;

    DrawLogoKanji();
}

void UPDATE(void) {
    UpdateTypewriter();

    // kanji done
    // now draw romaji
    if (menu_checkpoint == MENU_LOGO_KANJI &&
        TypewriterIsDone(kanji_upper_typewriter_idx) &&
        TypewriterIsDone(kanji_lower_typewriter_idx))
        DrawLogoRomaji();

    // romaji done
    // now draw map selector
    if (menu_checkpoint == MENU_LOGO_ROMAJI &&
        TypewriterIsDone(romaji_typewriter_idx))
        DrawOverlayMapSelector(MAP_SELECTOR_TILE_X, MAP_SELECTOR_TILE_Y);

    // map selector done
    // now draw map labels
    if (menu_checkpoint == MENU_OVERLAY_MAP_SELECTOR)
        DrawMapLabels();

    // map labels done
    // now detect keypresses
    if (menu_checkpoint == MENU_MAP_LABELS) {
        if (KEY_TICKED(J_RIGHT) || KEY_TICKED(J_UP)) { // previous
            if (selected_map_idx < MAP_COUNT - 1)
                ++selected_map_idx;
            else
                selected_map_idx = 0;

            DrawMapLabels();
        } else if (KEY_TICKED(J_LEFT) || KEY_TICKED(J_DOWN)) { // next
            if (selected_map_idx > 0)
                --selected_map_idx;
            else
                selected_map_idx = MAP_COUNT - 1;

            DrawMapLabels();
        }
    }
}

void DrawLogoKanji(void) {
    wait_vbl_done();

    font_offset = font_offsets.japanese_glyphs_font_offset;

    kanji_upper_typewriter_idx =
        DrawText((const unsigned char *)"ABEF", 4, 1, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // upper half of 大蛇
    kanji_lower_typewriter_idx =
        DrawText((const unsigned char *)"CDGH", 4, 2, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // lower half of 大蛇

    menu_checkpoint = MENU_LOGO_KANJI;
}

void DrawLogoRomaji(void) {
    wait_vbl_done();

    font_offset = font_offsets.yarara_font_primary_font_offset;

    romaji_typewriter_idx =
        DrawText((const unsigned char *)"OROCHI!", 9, 2, TEXT_ANCHOR_LEFT, 8,
                 TEXT_PRIMARY_PALETTE_IDX);

    menu_checkpoint = MENU_LOGO_ROMAJI;
}

void DrawOverlayMapSelector(uint8_t tile_x, uint8_t tile_y) {
    wait_vbl_done();

    font_offset = font_offsets.map_selector_tiles_font_offset;

    for (uint8_t row = 0; row < MAP_SELECTOR_ROW_COUNT; ++row)
        DrawText((const unsigned char *)map_selector_rows[row], tile_x,
                 tile_y++, TEXT_ANCHOR_LEFT, 0, MAP_SELECTOR_PALETTE_IDX);

    menu_checkpoint = MENU_OVERLAY_MAP_SELECTOR;
}

void DrawMapLabels(void) {
    wait_vbl_done();

    ResetTypewriter(map_id_typewriter_idx);
    ResetTypewriter(map_difficulty_typewriter_idx);
    ResetTypewriter(map_name_typewriter_idx);

    static unsigned char empty_filler[13];
    memset(empty_filler, ' ', 12); // 12 spaces
    empty_filler[12] = '\0';

    font_offset = font_offsets.yarara_font_primary_font_offset;

    // clear old map ID and difficulty
    DrawText(empty_filler, 4, 7, TEXT_ANCHOR_LEFT, 0, TEXT_PRIMARY_PALETTE_IDX);

    const Map *selected_map = &maps[selected_map_idx];

    static unsigned char map_id_label[6] = "Map ";
    map_id_label[4] = '1' + selected_map_idx;

    map_id_typewriter_idx = DrawText(map_id_label, 4, 7, TEXT_ANCHOR_LEFT, 2,
                                     TEXT_PRIMARY_PALETTE_IDX);

    static unsigned char map_difficulty_label[5];
    memset(map_difficulty_label, ':', 4); // unfilled star
    memset(map_difficulty_label, '?', selected_map->difficulty); // filled star
    map_difficulty_label[4] = '\0';

    map_difficulty_typewriter_idx =
        DrawText(map_difficulty_label, 16, 7, TEXT_ANCHOR_RIGHT, 2,
                 TEXT_PRIMARY_PALETTE_IDX);

    font_offset = font_offsets.yarara_font_secondary_font_offset;

    // clear old map name
    DrawText(empty_filler, 4, 10, TEXT_ANCHOR_LEFT, 0,
             TEXT_SECONDARY_PALETTE_IDX);

    static unsigned char map_title[MAP_NAME_MAX_LENGTH];
    memset(map_title, ' ',
           MAP_NAME_MAX_LENGTH - 1); // subtract 1 because of '\0'
    map_title[MAP_NAME_MAX_LENGTH - 1] = '\0';
    strncpy((char *)map_title, selected_map->name, strlen(selected_map->name));

    map_name_typewriter_idx = DrawText(map_title, 4, 10, TEXT_ANCHOR_LEFT, 3,
                                       TEXT_SECONDARY_PALETTE_IDX);

    menu_checkpoint = MENU_MAP_LABELS;
}
