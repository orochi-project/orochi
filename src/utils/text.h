#pragma once

#include "fonts/yarara_font_8x8.h"

#include <gb/cgb.h>
#include <stdbool.h>
#include <stdint.h>

/** The first VRAM tile to load the 8x8 Yarara Font sprites onto. */
#define YARARA_FONT_8X8_BASE_TILE 1
/** The first VRAM tile to load the 16x16 Orochi Kanji sprites onto. */
#define OROCHI_JP_16X16_BASE_TILE                                              \
    (YARARA_FONT_8X8_BASE_TILE + yarara_font_8x8_TILE_COUNT + 1)

/** The text anchor placement on the x-axis (left, center, or right). */
typedef enum {
    TEXT_ANCHOR_LEFT,   ///< Anchor the text on the left.
    TEXT_ANCHOR_CENTER, ///< Anchor the text on the center horizontally.
    TEXT_ANCHOR_RIGHT,  ///< Anchor the text on the right.
} TEXT_ANCHOR_X;

/** The text anchor placement on the y-axis (top, middle, or bottom). */
typedef enum {
    TEXT_ANCHOR_TOP,    ///< Anchor the text on the top.
    TEXT_ANCHOR_MIDDLE, ///< Anchor the text on the middle vertically.
    TEXT_ANCHOR_BOTTOM, ///< Anchor the text on the bottom.
} TEXT_ANCHOR_Y;

/**
 * The palette for the main accent color of text sprites.
 *
 * The actual color used for the text is defined in color slot 1.
 */
extern const palette_color_t text_accent_palette[4];

/**
 * The palette for the light-colored version of text sprites.
 *
 * The actual color used for the text is defined in color slot 1.
 */
extern const palette_color_t text_light_palette[4];

/**
 * Load the accent and light sprite palettes.
 *
 * - Palette 0: Accent
 * - Palette 1: Light
 */
void set_text_palettes(void);

/**
 * Load the 8x8 Yarara Font tiles as sprites.
 *
 * Sets the accent and light palettes for text as well.
 */
void load_yarara_font_8x8(void);

/**
 * Load the Orochi Japanese 16x16 logo sprites.
 *
 * Sets the accent and light palettes for text as well.
 */
void load_orochi_jp_16x16(void);

/**
 * Get the sequential glyph index (0-based) of a character within the
 * font sheet.
 *
 * Returns 0xFF if the character has no glyph.
 *
 * @param character The character to get the glyph index of.
 */
uint8_t get_glyph_index(char character);

/**
 * Draw text with the 8x8 Yarara Font using hardware sprites.
 *
 * Returns the next free sprite index after drawing the text.
 *
 * @param text          The string of text to draw onto the sprite layer.
 * @param start_x       The x-position of the text, anchored on the left/right.
 * @param start_y       The y-position of the text, anchored on the top.
 * @param anchor_x      Whether to anchor the text on the left, center, or
 * right.
 * @param anchor_y      Whether to anchor the text on the top, middle, or
 * bottom.
 * @param palette       The palette number to use for the text.
 * @param sprite_idx    The starting sprite index (0-39).
 */
uint8_t draw_sprite_text_8x8(const char *text, uint8_t start_x, uint8_t start_y,
                             TEXT_ANCHOR_X anchor_x, TEXT_ANCHOR_Y anchor_y,
                             uint8_t palette_number, uint8_t sprite_idx);
