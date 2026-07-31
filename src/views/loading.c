#include "views/loading.h"
#include "constants/levels.h"
#include "utils/text.h"

#include <gb/cgb.h>
#include <gb/gb.h>
#include <stdint.h>

const palette_color_t loading_screen_bkg_palette[8] = {
    RGB8(200, 192, 176), RGB8(200, 192, 176), RGB8(200, 192, 176),
    RGB8(200, 192, 176), RGB8(200, 192, 176), RGB8(200, 192, 176),
    RGB8(200, 192, 176), RGB8(200, 192, 176)};

void show_loading_screen(uint8_t *sprite_idx) {
    fill_bkg_rect(0, 0, 20, 18, 0);
    set_bkg_palette(0, 2, loading_screen_bkg_palette);

    *sprite_idx = 0;

    hide_sprites_range(*sprite_idx, MAX_HARDWARE_SPRITES);

    draw_sprite_text_8x8("Loading", 88, 88, TEXT_ANCHOR_CENTER,
                         TEXT_ANCHOR_MIDDLE, 0, sprite_idx);

    // shadow_OAM gets the x and y position of the previous sprite
    // +12 and +8 are hard coded values that work with the anchor. Try to find a
    // way to not need to hard code 12 and 8
    animate_loading_dots(sprite_idx, shadow_OAM[*sprite_idx - 1].x + 12,
                         shadow_OAM[*sprite_idx - 1].y + 8,
                         200); // loading animation
}

void animate_loading_dots(uint8_t *sprite_idx, uint8_t x_start, uint8_t y_start,
                          uint8_t speed) {
    uint8_t dot_count = 0;
    uint8_t starting_sprite_idx = *sprite_idx;

    while (1) {
        *sprite_idx = starting_sprite_idx;
        if (dot_count == 0)
            hide_sprites_range(starting_sprite_idx,
                               starting_sprite_idx +
                                   3); // hide dots if no dots needs to be shown

        // the dots to draw displayed in a string
        char dots[4] = "   ";
        for (uint8_t i = 0; i < dot_count; ++i) {
            dots[i] = '.';
        }

        draw_sprite_text_8x8(dots, x_start, y_start, TEXT_ANCHOR_LEFT,
                             TEXT_ANCHOR_MIDDLE, 0, sprite_idx);
        ++dot_count;

        if (dot_count == 4)
            dot_count = 0;

        delay(speed);
        vsync();
    }
}