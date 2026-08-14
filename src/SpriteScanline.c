#include "Banks/SetAutoBank.h"
#include "Scanline.h"
#include "SpriteManager.h"

void START(void) {
    scanline_sprite = THIS;
    THIS->coll_w = SCANLINE_COLLISION_WIDTH;

    ScanlineData *scanline_data = (ScanlineData *)THIS->custom_data;
    scanline_data->velocity = SCANLINE_START_VELOCITY;
    scanline_data->frozen = false;
}

void UPDATE(void) {
    ScanlineData *scanline_data = (ScanlineData *)THIS->custom_data;

    if (!scanline_data->frozen)
        THIS->x += scanline_data->velocity;

    if (THIS->x <= SCANLINE_BOUND_LEFT_X) {
        THIS->x = SCANLINE_BOUND_LEFT_X;
        scanline_data->velocity = -scanline_data->velocity;
    } else if (THIS->x >= SCANLINE_BOUND_RIGHT_X) {
        THIS->x = SCANLINE_BOUND_RIGHT_X;
        scanline_data->velocity = -scanline_data->velocity;
    }
}

void DESTROY(void) {}
