#pragma once

#include <stdbool.h>
#include <stdint.h>

/** The sprite index that points to the primary text color. */
#define TEXT_PRIMARY_PALETTE_IDX 1
/** The sprite index that points to the secondary text color. */
#define TEXT_SECONDARY_PALETTE_IDX 3

/** The maximum number of concurrent typewriters allowed. */
#define MAX_TYPEWRITERS 8

/** The text anchor placement on the x-axis (left, center, or right). */
typedef enum {
    TEXT_ANCHOR_LEFT,   ///< Anchor the text on the left.
    TEXT_ANCHOR_CENTER, ///< Anchor the text on the center horizontally.
    TEXT_ANCHOR_RIGHT,  ///< Anchor the text on the right.
} TextAnchor;

/**
 * Draw text as background tiles.
 *
 * @param text              The string of text to draw onto the background
 * layer.
 * @param tile_x            The x-position of the text.
 * @param tile_y            The y-position of the text.
 * @param text_anchor       Whether to anchor the text on the left, center, or
 * right.
 * @param typewriter_delay  The typewriter effect delay between characters (0 to
 * disable).
 * @param palette_idx       The palette number to use for the text.
 *
 * @return  The obtained typewriter slot index.
 */
int8_t DrawText(const unsigned char *text, uint8_t tile_x, uint8_t tile_y,
                TextAnchor text_anchor, uint8_t typewriter_delay,
                uint8_t palette_idx);

/** Update the ongoing typewriter effect. */
void UpdateTypewriter(void);

/**
 * Whether or not the typewriter effect is complete.
 *
 * @param typewriter_idx    The typewriter index to check.
 *
 * @return  Whether or not the typewriter is done.
 */
bool TypewriterIsDone(uint8_t typewriter_idx);

/**
 * Reset the typewriter state.
 *
 * @param typewriter_idx    The typewriter index to reset.
 */
void ResetTypewriter(uint8_t typewriter_idx);
