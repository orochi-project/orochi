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

/**
 * Display loading animation with 3 dots
 * 
 * @param sprite_idx    The starting sprite index for drawing the dot.
 * @param x_start       Starting X position to draw dots.
 * @param y_start       Starting Y position to draw dots.
 * @param speed         How much miliseconds to wait before next dot is drawn.
 */
void animate_loading_dots(uint8_t *sprite_idx, uint8_t x_start, uint8_t y_start, uint8_t speed);