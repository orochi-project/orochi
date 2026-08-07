#ifndef NOTES_H
#define NOTES_H

#include "Sprite.h"
#include <gbdk/platform.h>

/**
 * Displays a note on the screen using Sprite Manager
 *
 * @param note_type     Type of note to display; currently supports only
 * Horizontal and Vertical Tap Notes.
 * @param x_pos         X position on the screen.
 * @param y_pos         Y Position on the screen.
 * @param flipped       Whether a note should have the opposite click direction
 * of default.
 *
 */
Sprite *note(UINT8 note_type, UINT16 x_pos, UINT16 y_pos, UINT8 flipped);

#endif