#include "Banks/SetAutoBank.h"
#include "Print.h"
#include "Scroll.h"
// #include "SpriteManager.h"
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

/** Represents the possible logo states. */
typedef enum {
    MENU_LOGO_KANJI,
    MENU_LOGO_ROMAJI,
    MENU_OVERLAY_LEVEL_SELECTOR,
    MENU_MAP_LABELS,
} MenuCheckpoint;

/** The current menu state. */
static MenuCheckpoint menu_checkpoint;
/** The upper kanji typewriter index. */
static int8_t kanji_upper_typewriter_idx;
/** The lower kanji typewriter index. */
static int8_t kanji_lower_typewriter_idx;
/** The romaji typewriter index. */
static int8_t romaji_typewriter_idx;

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

    DrawLogoKanji();
}

void UPDATE(void) {
    UpdateTypewriter();

    // kanji done
    // now draw romaji
    if (menu_checkpoint == MENU_LOGO_KANJI &&
        TypewriterIsDone(kanji_upper_typewriter_idx) &&
        TypewriterIsDone(kanji_lower_typewriter_idx)) {
        DrawLogoRomaji();
    }

    if (menu_checkpoint == MENU_LOGO_ROMAJI &&
        TypewriterIsDone(romaji_typewriter_idx))
        DrawOverlayMapSelector(3, 5);

    if (menu_checkpoint == MENU_OVERLAY_LEVEL_SELECTOR) {
        DrawMapLabels();
    }
}

void DrawLogoKanji(void) {
    INIT_FONT(japanese_glyphs, PRINT_BKG);

    kanji_upper_typewriter_idx =
        DrawText("ABEF", 4, 1, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // upper half of 大蛇
    kanji_lower_typewriter_idx =
        DrawText("CDGH", 4, 2, TEXT_ANCHOR_LEFT, 10,
                 TEXT_PRIMARY_PALETTE_IDX); // lower half of 大蛇

    menu_checkpoint = MENU_LOGO_KANJI;
}

void DrawLogoRomaji(void) {
    INIT_FONT(yarara_font_primary, PRINT_BKG);

    romaji_typewriter_idx = DrawText("OROCHI!", 9, 2, TEXT_ANCHOR_LEFT, 10,
                                     TEXT_PRIMARY_PALETTE_IDX);

    menu_checkpoint = MENU_LOGO_ROMAJI;
}

void DrawOverlayMapSelector(uint8_t tile_x, uint8_t tile_y) {
    INIT_FONT(map_selector_tiles, PRINT_BKG);

    for (uint8_t row = 0; row < MAP_SELECTOR_ROW_COUNT; ++row) {
        PRINT(tile_x, tile_y, map_selector_rows[row]);
        ++tile_y;
    }

    menu_checkpoint = MENU_OVERLAY_LEVEL_SELECTOR;
}

void DrawMapLabels(void) {
    INIT_FONT(yarara_font_primary, PRINT_BKG);

    DrawText("Map 1", 4, 7, TEXT_ANCHOR_LEFT, 3, TEXT_PRIMARY_PALETTE_IDX);
    DrawText("?:::", 16, 7, TEXT_ANCHOR_RIGHT, 3, TEXT_PRIMARY_PALETTE_IDX);

    INIT_FONT(yarara_font_secondary, PRINT_BKG);

    DrawText("Isolation", 4, 10, TEXT_ANCHOR_LEFT, 3,
             TEXT_SECONDARY_PALETTE_IDX);

    menu_checkpoint = MENU_MAP_LABELS;
}
