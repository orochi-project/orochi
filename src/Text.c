#include "Text.h"
#include "Banks/SetAutoBank.h"
#include "Print.h"
#include <gbdk/platform.h>
#include <string.h>

/** Represents an instance of a typewriter. */
typedef struct {
    const unsigned char *text; ///< The final text to display.
    uint8_t length;            ///< The total length of the text.
    uint8_t tile_x;            ///< The tile x-position to start the text from.
    uint8_t tile_y;            ///< The tile y-position to place the text on.
    uint8_t delay;         ///< The interval over which each character appears.
    uint8_t timer;         ///< The mutable timer to track the progress of the
                           ///< typewriter instance.
    uint8_t palette_idx;   ///< The palette index to display the text with.
    uint8_t character_idx; ///< The current character index.
    bool active;
} Typewriter;

/** The typewriter slots. */
static Typewriter typewriters[MAX_TYPEWRITERS];

int8_t DrawText(const unsigned char *text, uint8_t tile_x, uint8_t tile_y,
                TextAnchor text_anchor, uint8_t typewriter_delay,
                uint8_t palette_idx) {
    // find free typewriter idx
    uint8_t typewriter_idx = 0;
    while (typewriter_idx < MAX_TYPEWRITERS &&
           typewriters[typewriter_idx].active)
        ++typewriter_idx;

    if (typewriter_idx >= MAX_TYPEWRITERS) // no free slot
        return -1;

    uint8_t text_length = strlen(text);

    if (text_anchor == TEXT_ANCHOR_CENTER)
        tile_x -= text_length / 2;
    else if (text_anchor == TEXT_ANCHOR_RIGHT)
        tile_x -= text_length;

    if (typewriter_delay) {
        typewriters[typewriter_idx].text = text;
        typewriters[typewriter_idx].length = text_length;
        typewriters[typewriter_idx].tile_x = tile_x;
        typewriters[typewriter_idx].tile_y = tile_y;
        typewriters[typewriter_idx].delay = typewriter_delay;
        typewriters[typewriter_idx].timer = typewriter_delay;
        typewriters[typewriter_idx].palette_idx = palette_idx;
        typewriters[typewriter_idx].character_idx = 0;
        typewriters[typewriter_idx].active = true;
    } else {
        PRINT(tile_x, tile_y, text);
#ifdef CGB
        for (uint8_t i = 0; i < text_length; ++i)
            set_bkg_attribute_xy(tile_x + i, tile_y, palette_idx & 0x07);
#endif
    }

    return typewriter_idx;
}

void UpdateTypewriter(void) {
    for (uint8_t typewriter_slot = 0; typewriter_slot < MAX_TYPEWRITERS;
         ++typewriter_slot) {
        Typewriter *typewriter = &typewriters[typewriter_slot];
        if (!typewriter->active ||
            typewriter->character_idx >= typewriter->length) // inactive or done
            continue;

        ++typewriter->timer;
        if (typewriter->timer < typewriter->delay)
            continue;

        typewriter->timer = 0;

        unsigned char buffer[2] = {typewriter->text[typewriter->character_idx],
                                   '\0'};
        PRINT(typewriter->tile_x + typewriter->character_idx,
              typewriter->tile_y, buffer);

#ifdef CGB
        set_bkg_attribute_xy(typewriter->tile_x + typewriter->character_idx,
                             typewriter->tile_y,
                             typewriter->palette_idx & 0x07);
#endif

        ++typewriter->character_idx;
    }
}

bool TypewriterIsDone(uint8_t typewriter_idx) {
    if (typewriter_idx >= MAX_TYPEWRITERS)
        return true;

    return !typewriters[typewriter_idx].active ||
           typewriters[typewriter_idx].character_idx >=
               typewriters[typewriter_idx].length;
}

void ResetTypewriter(uint8_t typewriter_idx) {
    if (typewriter_idx >= MAX_TYPEWRITERS)
        return;

    typewriters[typewriter_idx].active = false;
    typewriters[typewriter_idx].character_idx = 0;
    typewriters[typewriter_idx].timer = 0;
}
