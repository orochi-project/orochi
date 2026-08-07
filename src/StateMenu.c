#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "Maps.h"
#include "Music.h"
#include "Palette.h"
#include "Print.h"
#include "Scroll.h"
#include "Text.h"
#include "ZGBMain.h"
#include "gb/gb.h"
#include <stdint.h>
#include <string.h>

IMPORT_MAP(menu_background);
IMPORT_MAP(menu_map_selector);

IMPORT_TILES(japanese_glyphs);
IMPORT_TILES(yarara_font_primary);
IMPORT_TILES(yarara_font_secondary);

DECLARE_MUSIC(isolation);

extern const palette_color_t menu_map_selector_palettes[4];

/** The starting x-position to place the map selector. */
#define MAP_SELECTOR_TILE_X 3
/** The starting y-position to place the map selector. */
#define MAP_SELECTOR_TILE_Y 5
/** The palette index that points to the map selector overlay colors. */
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
    InitScroll(BANK(menu_background), &menu_background, 0, 0);

    PlayMusic(isolation, LOOP);

    INIT_FONT(japanese_glyphs, PRINT_BKG);
    font_offsets.japanese_glyphs_font_offset = font_offset;

    INIT_FONT(yarara_font_primary, PRINT_BKG);
    font_offsets.yarara_font_primary_font_offset = font_offset;

    INIT_FONT(yarara_font_secondary, PRINT_BKG);
    font_offsets.yarara_font_secondary_font_offset = font_offset;

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
        } else if (KEY_TICKED(J_START))
            SetState(StateGame);
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

    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(BANK(menu_map_selector));

    // set palette for level selector manually
    SetPalette(BG_PALETTE, MAP_SELECTOR_PALETTE_IDX, 1,
               menu_map_selector_palettes, BANK(menu_map_selector));

    // load the tile graphics
    // LoadMap() basically does this, but we need to do this manually to be able
    // to patch the attributes and thus set the palette correctly
    uint16_t map_offset =
        ScrollSetTiles(last_tile_loaded, menu_map_selector.tiles_bank,
                       menu_map_selector.tiles);

    // patch each tile's attribute byte to point at our slot instead of 0,
    // since the exported asset hardcodes palette index 0 for whatever reason
    const unsigned char *data = menu_map_selector.data;
    const unsigned char *attributes = menu_map_selector.attributes;

    for (uint8_t y = 0; y < menu_map_selector.height; ++y)
        for (uint8_t x = 0; x < menu_map_selector.width; ++x) {
            uint8_t patched_attributes =
                (*attributes & ~0x07) | MAP_SELECTOR_PALETTE_IDX;

            UpdateMapTile(TARGET_BKG, tile_x + x, tile_y + y, map_offset, *data,
                          &patched_attributes);

            ++data;
            ++attributes;
        }

    SWITCH_ROM(_saved_bank);

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
