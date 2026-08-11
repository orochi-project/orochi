#include "Banks/SetAutoBank.h"
#include "Music.h"
#include <gbdk/platform.h>

/**
 * The frame interval over which exactly one audio tick should be skipped.
 *
 * WARN: Hardcoding this is probably not gonna be a great idea eventually.
 * TODO: Add an attribute to each map that can be passed into here (?) to allow
 * for dynamic skip intervals. (Though this would probably need a lot of manual
 * measurement/calibration.)
 */
#define AUDIO_SKIP_INTERVAL 145

/** The counter to track when one audio tick must be skipped. */
static uint8_t skip_counter = 0;

void InitGameAudio(void) NONBANKED {
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

void TickGameAudio(void) NONBANKED {
    if (music_paused)
        return;
    if (last_music_bank == SFX_STOP_BANK)
        return;

    // skip once every AUDIO_SKIP_INTERVAL ticks
#if AUDIO_SKIP_INTERVAL > 0
    ++skip_counter;
    if (skip_counter >= AUDIO_SKIP_INTERVAL) {
        skip_counter = 0;
        return; // skip to slow down audio
    }
#endif

    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(last_music_bank);

    hUGE_dosound();

    SWITCH_ROM(_saved_bank);
}
