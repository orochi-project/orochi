#pragma once

#include <gb/cgb.h>
#include <stdint.h>

/**
 * The palette for the loading screen background.
 *
 * Consists of 8 colors to replace both background palettes.
 */
extern const palette_color_t loading_screen_bkg_palette[8];

/**
 * Display the loading screen sequence.
 *
 * Modifies the sprite index to the next free tile after drawing.
 *
 * @param sprite_idx    The starting sprite index (0-39).
 */
void show_loading_screen(uint8_t *sprite_idx);