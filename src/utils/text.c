#include "text.h"
#include "graphics/fonts/orochi_jp_16x16.h"
#include "graphics/fonts/yarara_font_8x8.h"

#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdint.h>

const palette_color_t text_accent_palette[4] = {
    RGB8(0, 0, 0), RGB8(200, 32, 32), RGB8(0, 0, 0), RGB8(0, 0, 0)};

const palette_color_t text_light_palette[4] = {
    RGB8(0, 0, 0), RGB8(232, 224, 208), RGB8(0, 0, 0), RGB8(0, 0, 0)};

void set_text_palettes(void) {
    set_sprite_palette(0, 1, text_accent_palette);
    set_sprite_palette(1, 1, text_light_palette);
}

void load_yarara_font_8x8(void) {
    set_sprite_data(YARARA_FONT_8X8_BASE_TILE, yarara_font_8x8_TILE_COUNT,
                    yarara_font_8x8_tiles);
    set_text_palettes();
}

void load_orochi_jp_16x16(void) {
    set_sprite_data(OROCHI_JP_16X16_BASE_TILE, orochi_jp_16x16_TILE_COUNT,
                    orochi_jp_16x16_tiles);
    set_text_palettes();
}

uint8_t get_glyph_index(char character) {
    uint8_t index;

    if (character >= 'a' && character <= 'z')
        index = character - 'a';
    else if (character >= 'A' && character <= 'Z')
        index = character - 'A';
    else if (character >= '0' && character <= '9')
        index = (character - '0') + 26;
    else {
        switch (character) {
        case '!':
            index = 36;
            break;
        case ':':
            index = 37;
            break;
        case '?':
            index = 38;
            break;
        case '/':
            index = 39;
            break;
        case '-':
            index = 40;
            break;
        case ',':
            index = 41;
            break;
        case '.':
            index = 42;
            break;
        case '<':
            index = 43;
            break;
        case '>':
            index = 44;
            break;
        case '+':
            index = 45;
            break;
        case '*':
            index = 46;
            break;
        case '~': // 千
            index = 47;
            break;
        case '@': // 本
            index = 48;
            break;
        case '#': // 桜
            index = 49;
            break;
        default:
            return 0xFF;
        }
    }

    return index;
}

void draw_sprite_text_8x8(const char *text, uint8_t start_x, uint8_t start_y,
                          TEXT_ANCHOR_X anchor_x, TEXT_ANCHOR_Y anchor_y,
                          uint8_t palette_number, uint8_t *sprite_idx) {
    uint8_t x = start_x;
    uint8_t y = start_y;

    uint8_t text_length = 0;

    for (uint8_t i = 0; text[i] != '\0'; ++i)
        ++text_length;

    uint8_t text_width = text_length * 8;

    // We do not need to handle TEXT_ANCHOR_LEFT because it is assumed to be the
    // default when the x variable was initialized.
    if (anchor_x == TEXT_ANCHOR_CENTER)
        x -= text_width / 2;
    else if (anchor_x == TEXT_ANCHOR_RIGHT)
        x -= text_width;

    // Likewise for TEXT_ANCHOR_TOP for the y variable
    if (anchor_y == TEXT_ANCHOR_MIDDLE)
        y -= 4;
    else if (anchor_y == TEXT_ANCHOR_BOTTOM)
        y -= 8;

    for (uint8_t i = 0; text[i] != '\0' && *sprite_idx + 1 < 40; ++i) {
        uint8_t glyph_index = get_glyph_index(text[i]);

        // Treat unmapped characters as spaces
        if (glyph_index == 0xFF) {
            x += 8;
            continue;
        }

        glyph_index += glyph_index + 1;

        *sprite_idx += move_metasprite_ex(
            yarara_font_8x8_metasprites[glyph_index], YARARA_FONT_8X8_BASE_TILE,
            palette_number, *sprite_idx, x, y);

        x += 8;
    }
}
