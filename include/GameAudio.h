#pragma once

#include <stdint.h>

/** The frame interval over which exactly one audio tick should be skipped. */
extern uint16_t audio_skip_interval;

/** Initialize the audio for the game. */
void InitGameAudio(void);

/** Restore the default CrossZGB music settings. */
void RestoreDefaultAudio(void);

/** Step forward by one tick in the currently loaded audio. */
void TickGameAudio(void);
