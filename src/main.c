#include <gb/cgb.h>
#include <gb/gb.h>
#include <stdint.h>

#include "hUGEDriver.h"
#include "views/menu.h"

extern const hUGESong_t thirdboot;
BANKREF_EXTERN(thirdboot) // external ref to the bank with the track

/**
 * The currently selected audio bank.
 *
 * Used by the VBlank interrupt handler to make sure the CPU switches to the
 * correct ROM bank before trying to process audio data.
 *
 * If the currently selected audio bank is 0, then the audio is in an
 * uninitialized or stopped state, and the audio tick is skipped.
 */
static uint8_t current_audio_bank = 0;

/**
 * The VBlank Interrupt Service Routine wrapper for audio playback.
 *
 * Executed every frame during the Game Boy's VBlank phase. It
 * backs up the current ROM bank, switches to the designated audio bank, ticks
 * the hUGEDriver sound engine, and then restores the original ROM bank to
 * prevent the main game loop from crashing.
 *
 * Returns early if `current_audio_bank` is 0.
 */
void play_sound_vbl(void) {
    if (current_audio_bank == 0)
        return;

    uint8_t _previous_bank = _current_bank;

    SWITCH_ROM(current_audio_bank);
    hUGE_dosound();
    SWITCH_ROM(_previous_bank);
}

/** The main entry point for the game. */
void main(void) {
    NR52_REG = 0x80; // master sound on
    NR50_REG = 0xFF; // max volume
    NR51_REG = 0x77; // enable sound channels

    uint8_t current_sprite = 0;

    DISPLAY_OFF;

    load_menu();

    DISPLAY_ON;

    // pre-register play_sound_vbl() as a function to run on VBlank
    add_VBL(play_sound_vbl);

    CRITICAL {
        uint8_t song_bank = BANK(thirdboot);
        current_audio_bank = song_bank;

        uint8_t _previous_bank = _current_bank;

        SWITCH_ROM(song_bank);
        hUGE_init(&thirdboot);
        SWITCH_ROM(_previous_bank);
    }

    run_menu_loop(&current_sprite);
}
