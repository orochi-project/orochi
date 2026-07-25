#include "views/menu.h"
#include "backgrounds/menu_background.h"
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

    // The starting x-position is 52, which allows the logo to be properly
    // centered. The starting y-position of the logo is 0 because we want to
    // slide the logo down after.
    uint8_t initial_sprite_idx = *sprite_idx;

    *sprite_idx = draw_menu_logo(*sprite_idx, 52, 0);
    slide_down_logo(32, 6, initial_sprite_idx, *sprite_idx);

    // TODO: This is a dummy level selector. This shall be replaced with a
    // working one later.
    *sprite_idx = draw_sprite_text_8x8("LEVEL 1", 44, 80, TEXT_ANCHOR_LEFT,
                                       TEXT_ANCHOR_TOP, 0, *sprite_idx);
    *sprite_idx = draw_sprite_text_8x8("<", 44, 96, TEXT_ANCHOR_LEFT,
                                       TEXT_ANCHOR_TOP, 1, *sprite_idx);
    *sprite_idx = draw_sprite_text_8x8("ABCDEFGH", 92, 96, TEXT_ANCHOR_CENTER,
                                       TEXT_ANCHOR_TOP, 1, *sprite_idx);
    *sprite_idx = draw_sprite_text_8x8(">", 140, 96, TEXT_ANCHOR_RIGHT,
                                       TEXT_ANCHOR_TOP, 1, *sprite_idx);
    *sprite_idx = draw_sprite_text_8x8("*++++", 140, 112, TEXT_ANCHOR_RIGHT,
                                       TEXT_ANCHOR_TOP, 0, *sprite_idx);

    /* This loop should eventually break on certain conditions.
     * (For example, when the user enters a level.)
     *
     * Omitted for now because there is nothing to loop at the moment. And, of
     * course, we do want the function to return.
     */
    // while (1) {
    //     vsync();
    // }
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

uint8_t draw_menu_logo(uint8_t sprite_idx, uint8_t start_x, uint8_t start_y) {
    uint8_t current_sprite = sprite_idx;

    current_sprite +=
        move_metasprite_ex(orochi_jp_16x16_metasprites[0], FONT_2_BASE_TILE, 0,
                           current_sprite, start_x, start_y); // 大
    current_sprite += move_metasprite_ex(
        orochi_jp_16x16_metasprites[1], FONT_2_BASE_TILE, 0, current_sprite,
        start_x + 16, start_y); // 蛇
                                // +16 to next tile
    current_sprite = draw_sprite_text_8x8("OROCHI", start_x + 36, start_y - 4,
                                          TEXT_ANCHOR_LEFT, TEXT_ANCHOR_TOP, 0,
                                          current_sprite);

    return current_sprite;
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
