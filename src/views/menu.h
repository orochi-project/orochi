#pragma once

#include <stdint.h>

extern uint8_t level_name_start;
extern uint8_t level_name_end;

/** Load and draw the menu background map. */
void load_menu_background(void);

/** Draw the menu background. */
void draw_menu_background(void);

/**
 * Draw the Orochi logo onto the menu header.
 *
 * Modifies the sprite index to the next free tile after drawing.
 *
 * @param start_x     The x-position of the logo, anchored on the left.
 * @param start_y     The y-position of the logo, anchored on the bottom.
 * @param sprite_idx  The starting sprite index (0-39).
 */
void draw_menu_logo(uint8_t start_x, uint8_t start_y, uint8_t *sprite_idx);

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
 * Draw the current level onto the screen.
 *
 * Removes old sprites and draws new ones in their positions.
 *
 * Returns the next free sprite index.
 *
 * @param sprite_start  Where to start drawing the sprite.
 * @param sprite_end    Where the sprite drawing ends.
 * @param level         The level to show.
 */
uint8_t switch_selected_level(uint8_t sprite_start, uint8_t sprite_end,
                              uint8_t level_id);

/**
 * Animate the logo moving out when a level is selected by the player
 *
 * @param title_speed       The speed that the title moves out of the screen
 * @param level_info_speed  The speed that the level information moves out of the screen
 * @param menu_end          The sprite index of the tile after the last menu sprite.
 */
void slide_out_sprites(uint8_t title_speed, uint8_t level_info_speed,
                       uint8_t menu_end);