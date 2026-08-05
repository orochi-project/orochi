#include "ZGBMain.h"
#include "Math.h"
#include <stdint.h>

uint8_t next_state = StateMenu;

uint8_t GetTileReplacement(uint8_t *tile_ptr, uint8_t *tile) {
    if (current_state == StateMenu) {
        if (U_LESS_THAN(255 - (uint16_t)*tile_ptr, N_SPRITE_TYPES)) {
            *tile = 0;
            return 255 - (uint16_t)*tile_ptr;
        }

        *tile = *tile_ptr;
    }

    return 255;
}
