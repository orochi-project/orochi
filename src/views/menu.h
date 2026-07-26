#pragma once

#include <stdint.h>

/** Load and draw the menu background map. */
void load_menu_background(void);

/** Draw the menu background. */
void draw_menu_background(void);

/**
 * Draw the Orochi logo onto the menu header.
 *
 * Returns the next free sprite index after drawing the logo.
 *
 * @param sprite_idx  The starting sprite index (0-39).
 * @param start_x     The x-position of the logo, anchored on the left.
 * @param start_y     The y-position of the logo, anchored on the bottom.
 */
uint8_t draw_menu_logo(uint8_t sprite_idx, uint8_t start_x, uint8_t start_y);

/** Load the menu and its resources. */
void load_menu(void);

/**
 * Slide down the logo a given distance at a given speed.
 *
 * @param distance_y        The distance, in pixels, to slide down.
 * @param speed             The speed, in pixels per frame, at which the logo
 * slides down.
 * @param start_sprite_idx  The sprite index of the first logo sprite.
 * @param end_sprite_idx    The sprite index of the last logo sprite.
 */
void slide_down_logo(uint8_t distance_y, uint8_t speed,
                     uint8_t start_sprite_idx, uint8_t end_sprite_idx);

/**
 * Run the menu loop.
 *
 * Modifies the sprite index in-place using a pointer.
 *
 * @param sprite_idx The starting sprite index (0-39).
 */
void run_menu_loop(uint8_t *sprite_idx);

/**
 * Level selection: Draws the current level selected onto the screen. 
 
 * Removes old sprites and draws new ones in its place.
 
 * @param menu_sprite_start Where to start drawing the sprite
 * @param menu_sprite_end Where the sprite drawing ends. Combined with hide_sprite_range(), removes old sprites.
 * @param level_to_draw What level should be retrieved through the levels struct and displayed on screen.
 */
uint8_t draw_new_level_selected(uint8_t menu_sprite_start, uint8_t menu_sprite_end, uint8_t level_to_draw);
