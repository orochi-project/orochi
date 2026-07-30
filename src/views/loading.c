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

    draw_sprite_text_8x8("Loading...", 88, 88, TEXT_ANCHOR_CENTER,
                         TEXT_ANCHOR_MIDDLE, 0, sprite_idx);
}