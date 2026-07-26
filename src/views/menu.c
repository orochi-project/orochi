#include "views/menu.h"
#include "backgrounds/menu_background.h"
#include "constants/levels.h"
#include "fonts/orochi_jp_16x16.h"
#include "utils/text.h"

#include <gb/gb.h>
#include <gb/hardware.h>
#include <gb/metasprites.h>
#include <stdint.h>

void load_menu(void) {
    SPRITES_8x16;

    hide_sprites_range(0, MAX_HARDWARE_SPRITES);

    load_menu_background();
    load_yarara_font_8x8();
    load_orochi_jp_16x16();

    SHOW_SPRITES;
}

void run_menu_loop(uint8_t *sprite_idx) {
    draw_menu_background();

    uint8_t initial_sprite_idx = *sprite_idx;

    draw_menu_logo(52, 0, sprite_idx);
    slide_down_logo(32, 6, initial_sprite_idx, *sprite_idx);

    uint8_t selected_level_idx = 0;
    uint8_t menu_start = *sprite_idx;

    // Initial level (level 1). Returns next free sprite location as menu end.
    uint8_t menu_end =
        switch_selected_level(menu_start, menu_start, selected_level_idx);

    uint8_t prev_joy = 0;

    while (1) {
        uint8_t joy = joypad();

        // Bit logic: Pressed is only true the first time you press; prevents
        // holding and accidental spam.
        uint8_t pressed = joy & ~prev_joy; // buttons that are being pressed and
                                           // were NOT pressed last frame

        if (pressed & J_RIGHT) {
            if (selected_level_idx < LEVEL_COUNT - 1)
                ++selected_level_idx;
            else
                selected_level_idx = 0;

            menu_end =
                switch_selected_level(menu_start, menu_end, selected_level_idx);
        }

        if (pressed & J_LEFT) {
            if (selected_level_idx > 0)
                --selected_level_idx;
            else
                selected_level_idx = LEVEL_COUNT - 1;

            menu_end =
                switch_selected_level(menu_start, menu_end, selected_level_idx);
        }

        prev_joy = joy;

        vsync();
    }
}

void load_menu_background(void) {
    set_bkg_data(0, menu_background_TILE_COUNT,
                 menu_background_tiles); // load menu bkg tiles into VRAM
    set_bkg_palette(
        0, menu_background_PALETTE_COUNT,
        menu_background_palettes); // set palette 0 to the menu bkg palettes
}

void draw_menu_background(void) {
    set_bkg_tiles(0, 0, 20, 18, menu_background_map);
    set_bkg_attributes(0, 0, 20, 18, menu_background_map_attributes);
}

void draw_menu_logo(uint8_t start_x, uint8_t start_y, uint8_t *sprite_idx) {
    *sprite_idx += move_metasprite_ex(orochi_jp_16x16_metasprites[0],
                                      OROCHI_JP_16X16_BASE_TILE, 0, *sprite_idx,
                                      start_x, start_y); // 大
    *sprite_idx += move_metasprite_ex(
        orochi_jp_16x16_metasprites[1], OROCHI_JP_16X16_BASE_TILE, 0,
        *sprite_idx, start_x + 16, start_y); // 蛇
                                             // +16 to next tile
    draw_sprite_text_8x8("OROCHI", start_x + 36, start_y - 4, TEXT_ANCHOR_LEFT,
                         TEXT_ANCHOR_TOP, 0, sprite_idx);
}

void slide_down_logo(uint8_t distance_y, uint8_t speed,
                     uint8_t start_sprite_idx, uint8_t end_sprite_idx) {
    for (uint8_t current_sprite = start_sprite_idx;
         current_sprite <= end_sprite_idx; ++current_sprite) {
        uint8_t distance_moved = 0;
        uint8_t target_distance = distance_y;

        // Sprites 0-3 are the four indices used for the 16x16 kanji sprites 大
        // and 蛇. Since the 8x8 character sprites are technically rendered as
        // 8x16 sprites with the lower half blank, we must add the missing 8
        // pixels to compensate.
        if (current_sprite > 3)
            target_distance += 8;

        while (distance_moved < target_distance) {
            uint8_t step = speed;

            if (distance_moved + speed > target_distance)
                step = target_distance - distance_moved;

            scroll_sprite(current_sprite, 0, step);

            distance_moved += step;

            vsync();
        }
    }
}

uint8_t switch_selected_level(uint8_t sprite_start, uint8_t sprite_end,
                              uint8_t level_idx) {
    if (sprite_end >= MAX_HARDWARE_SPRITES)
        sprite_end = MAX_HARDWARE_SPRITES - 1;

    // hides the previous level selection sprites so new ones can be drawn
    hide_sprites_range(sprite_start, sprite_end);

    uint8_t current_sprite = sprite_start;

    const struct Level *level = &levels[level_idx];

    // change Map 1 --> Map 2 etc, less resource heavy on gameboy than
    // sprintf method
    // for now we are assuming there are no more than 9 total levels
    char map_text[6] = "MAP _";
    map_text[4] = '1' + level_idx;

    char difficulty_text[6] = "+++++";
    for (uint8_t i = 0; i < level->difficulty; ++i)
        difficulty_text[i] = '*';

    draw_sprite_text_8x8(map_text, 44, 80, TEXT_ANCHOR_LEFT, TEXT_ANCHOR_TOP, 0,
                         &current_sprite);

    draw_sprite_text_8x8("<", 44, 96, TEXT_ANCHOR_LEFT, TEXT_ANCHOR_TOP, 1,
                         &current_sprite);

    draw_sprite_text_8x8(level->name, 92, 96, TEXT_ANCHOR_CENTER,
                         TEXT_ANCHOR_TOP, 1, &current_sprite);

    draw_sprite_text_8x8(">", 140, 96, TEXT_ANCHOR_RIGHT, TEXT_ANCHOR_TOP, 1,
                         &current_sprite);

    draw_sprite_text_8x8(difficulty_text, 140, 112, TEXT_ANCHOR_RIGHT,
                         TEXT_ANCHOR_TOP, 0, &current_sprite);

    return current_sprite;
}
