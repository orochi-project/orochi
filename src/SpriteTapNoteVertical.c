#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "ZGBMain.h"

#define TIMER_INTERVAL 15

typedef struct {
    uint8_t current_frame;
    uint8_t timer;
} CUSTOM_DATA;

void START(void) {
    CUSTOM_DATA *data = (CUSTOM_DATA *)THIS->custom_data;
    data->current_frame = 0;
    data->timer = TIMER_INTERVAL;
}

void UPDATE(void) {
    CUSTOM_DATA *data = (CUSTOM_DATA *)THIS->custom_data;

    --data->timer;

    // wait TIMER_INTERVAL until running the next frame animation
    if (data->timer == 0) {
        if (data->current_frame < 4)
            ++data->current_frame;
        else
            data->current_frame = 0;

        data->timer = TIMER_INTERVAL;
        SetFrame(THIS, data->current_frame);
    }

    // If the note collides with the scanline and the correct direction is
    // pressed, delete the note
    if (CheckCollision(THIS, scanline_sprite)) {
        if ((THIS->mirror && KEY_TICKED(J_DOWN)) ||
            (!THIS->mirror && KEY_TICKED(J_UP))) {
            SpriteManagerRemoveSprite(THIS);
            return;
        }
    }
}

void DESTROY(void) {}
