#include "views/menu.h"

#include "hUGEDriver.h"
#include <gb/cgb.h>
#include <gb/gb.h>
#include <stdint.h>

/**
 * The menu loop song.
 *
 * From /res/audio/soundtrack/menu/menu_loop.uge
 */
extern const hUGESong_t isolation;

void main(void) {
    uint8_t current_sprite = 0;

    NR52_REG = 0x80; // master sound on
    NR50_REG = 0xFF; // maximum volume for left/right speakers
    NR51_REG = 0xFF; // enable sound
                     //
    CRITICAL {
        hUGE_init(&isolation);
        add_VBL(hUGE_dosound);
    }

    DISPLAY_OFF;

    load_menu();

    DISPLAY_ON;

    run_menu_loop(&current_sprite);
}
