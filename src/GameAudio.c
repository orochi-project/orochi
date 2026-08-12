#include "GameAudio.h"
#include "Music.h"

uint16_t audio_skip_interval;

/** The counter to track when one audio tick must be skipped. */
static uint8_t skip_counter = 0;

void InitGameAudio(void) {
    sfx_sound_init();
    sfx_sound_cut();

    CRITICAL {
        // CrossZGB's default INIT_MUSIC() in main.c already sets up the timer.
        // We must override it in order to use our own flags.
        TAC_REG = 0x00;       // stop timer
        IE_REG &= ~TIM_IFLAG; // disable timer interrupt mask
        IF_REG &= ~TIM_IFLAG; // clear timer interrupts

        set_interrupts(IE_REG | VBL_IFLAG);
    }

    INIT_MUSIC_DRIVER();
}

void TickGameAudio(void) {
    if (music_paused)
        return;
    if (last_music_bank == SFX_STOP_BANK)
        return;

    // skip once every audio_skip_interval ticks
    if (audio_skip_interval)
        if (++skip_counter >= audio_skip_interval) {
            skip_counter = 0;
            return; // skip to slow down audio
        }

    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(last_music_bank);

    hUGE_dosound();

    SWITCH_ROM(_saved_bank);
}
