#include "Banks/SetAutoBank.h"
#include "Print.h"
#include "Scroll.h"
// #include "SpriteManager.h"
#include "Keys.h"
#include "Text.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include <stdint.h>

IMPORT_MAP(menu_map);

IMPORT_TILES(japanese_glyphs);
IMPORT_TILES(yarara_font_primary);
IMPORT_TILES(yarara_font_secondary);
IMPORT_TILES(map_selector_tiles);

/** The number of map selector rows to display. */
#define MAP_SELECTOR_ROW_COUNT 11
/** The palette index that points to the map selector tile colors. */
#define MAP_SELECTOR_PALETTE_IDX 3

/** Represents the possible logo states. */
typedef enum {
    MENU_LOGO_KANJI,
    MENU_LOGO_ROMAJI,
    MENU_OVERLAY_LEVEL_SELECTOR,
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

static uint8_t current_selected_level = 1;

/** The map selector row tiles as font characters. */
const unsigned char *map_selector_rows[MAP_SELECTOR_ROW_COUNT] = {
    "AEEEEEEEEEEEEC", "GIIIIIIIIIIIIH", "GIIIIIIIIIIIIH", "GIIIIIIIIIIIIH",
    "JJJJJJJJJJJJJJ", "JJJJJJJJJJJJJJ", "JJJJJJJJJJJJJJ", "GKLMNIIIISTUVH",
    "GOPQRIIIIWXYZH", "GII01234567IIH", "BFFFFFFFFFFFFD",
};

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
        DrawOverlayMapSelector(3, 5);

    // map selector done
    // now draw map labels
    if (menu_checkpoint == MENU_OVERLAY_LEVEL_SELECTOR)
        DrawMapLabels();

    if (menu_checkpoint == MENU_MAP_LABELS) {
        if (KEY_PRESSED(J_UP)) {
            ++current_selected_level;
            DrawMapLabels();
        }
    }
}

void DrawLogoKanji(void) {
    font_offset = font_offsets.japanese_glyphs_font_offset;

    kanji_upper_typewriter_idx =
        DrawText("ABEF", 4, 1, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // upper half of 大蛇
    kanji_lower_typewriter_idx =
        DrawText("CDGH", 4, 2, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // lower half of 大蛇

    menu_checkpoint = MENU_LOGO_KANJI;
}

void DrawLogoRomaji(void) {
    font_offset = font_offsets.yarara_font_primary_font_offset;

    romaji_typewriter_idx = DrawText("OROCHI!", 9, 2, TEXT_ANCHOR_LEFT, 10,
                                     TEXT_PRIMARY_PALETTE_IDX);

    menu_checkpoint = MENU_LOGO_ROMAJI;
}

void DrawOverlayMapSelector(uint8_t tile_x, uint8_t tile_y) {
    font_offset = font_offsets.map_selector_tiles_font_offset;

    for (uint8_t row = 0; row < MAP_SELECTOR_ROW_COUNT; ++row) {
        DrawText(map_selector_rows[row], tile_x, tile_y, TEXT_ANCHOR_LEFT, 0,
                 MAP_SELECTOR_PALETTE_IDX);
        ++tile_y;
    }

    menu_checkpoint = MENU_OVERLAY_LEVEL_SELECTOR;
}

void DrawMapLabels(void) {
    font_offset = font_offsets.yarara_font_primary_font_offset;

    static unsigned char map_number_label[6] = "Map ";
    map_number_label[4] = '0' + current_selected_level;

    DrawText(map_number_label, 4, 7, TEXT_ANCHOR_LEFT, 3,
             TEXT_PRIMARY_PALETTE_IDX);
    DrawText("?:::", 16, 7, TEXT_ANCHOR_RIGHT, 3, TEXT_PRIMARY_PALETTE_IDX);

    font_offset = font_offsets.yarara_font_secondary_font_offset;

    DrawText("Isolation", 4, 10, TEXT_ANCHOR_LEFT, 3,
             TEXT_SECONDARY_PALETTE_IDX);

    menu_checkpoint = MENU_MAP_LABELS;
}
