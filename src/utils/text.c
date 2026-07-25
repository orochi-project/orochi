#include "text.h"
#include "fonts/orochi_jp_16x16.h"
#include "fonts/yarara_font_8x8.h"

#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdint.h>

const palette_color_t text_accent_palette[4] = {
    RGB8(232, 224, 208), RGB8(200, 32, 32), RGB8(0, 0, 0), RGB8(0, 0, 0)};

const palette_color_t text_light_palette[4] = {
    RGB8(232, 224, 208), RGB8(232, 224, 208), RGB8(0, 0, 0), RGB8(0, 0, 0)};

void load_yarara_font_8x8(void) {
    set_sprite_data(FONT_1_BASE_TILE, yarara_font_8x8_TILE_COUNT,
                    yarara_font_8x8_tiles);
    SET_TEXT_PALETTES();
}

void load_orochi_jp_16x16(void) {
    set_sprite_data(FONT_2_BASE_TILE, orochi_jp_16x16_TILE_COUNT,
                    orochi_jp_16x16_tiles);
    SET_TEXT_PALETTES();
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
        case '\\':
            index = 39;
            break;
        case '=':
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
        case '大':
            index = 45;
            break;
        case '蛇':
            index = 46;
            break;
        default:
            return 0xFF;
        }
    }

    return index;
}

uint8_t draw_sprite_text_8x8(const char *text, uint8_t start_x, uint8_t start_y,
                             TEXT_ANCHOR_X anchor_x, TEXT_ANCHOR_Y anchor_y,
                             uint8_t palette_number, uint8_t sprite_idx) {
    uint8_t current_sprite = sprite_idx;

    uint8_t x = start_x;
    uint8_t y = start_y;

    uint8_t text_length = 0;

    for (uint8_t i = 0; text[i] != '\0'; ++i)
        ++text_length;

    uint8_t text_width = text_length * 8;

    switch (anchor_x) {
    case TEXT_ANCHOR_CENTER:
        x -= text_width / 2;
        break;

    case TEXT_ANCHOR_RIGHT:
        x -= text_width;
        break;
    }

    switch (anchor_y) {
    case TEXT_ANCHOR_MIDDLE:
        y -= 4;
        break;

    case TEXT_ANCHOR_BOTTOM:
        y -= 8;
        break;
    }

    for (uint8_t i = 0; text[i] != '\0' && current_sprite + 1 < 40; ++i) {
        uint8_t glyph_index = get_glyph_index(text[i]);

        // Treat unmapped characters as spaces
        if (glyph_index == 0xFF) {
            x += 8;
            continue;
        }

        glyph_index += glyph_index + 1;

        current_sprite += move_metasprite_ex(
            yarara_font_8x8_metasprites[glyph_index], FONT_1_BASE_TILE,
            palette_number, current_sprite, x, y);

        x += 8;
    }

    return current_sprite; // next free sprite index
}
