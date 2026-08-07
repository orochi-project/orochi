#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include <stdint.h>

static int8_t velocity = 1;

void START(void) { scanline_sprite = THIS; }

void UPDATE(void) {
    if (THIS->x + velocity <= 9) {
        velocity *= -1;
        THIS->x = 9;
    } else if (THIS->x + velocity >= 151) {
        velocity *= -1;
        THIS->x = 151;
    }
    THIS->x += velocity;
}

void DESTROY(void) {}

// 9 ---> 151