#pragma once

#include "Notes.h"
#include <stdint.h>

/** Play the song for the currently selected map. */
void PlayCurrentMapSong(void) BANKED;

/** Initialize the audio for the game. */
void InitGameAudio(void) BANKED;

/** Restore the default CrossZGB music settings. */
void RestoreDefaultAudio(void) BANKED;

/** Step forward by one tick in the currently loaded audio. */
void TickGameAudio(void) NONBANKED;

/**
 * Record a note's hit grade.
 *
 * @param grade  The grade the note was graded with.
 */
void RegisterNoteHit(NoteHitGrade grade) BANKED;

/** Reset all hit grade counts back to zero. */
void ResetAccuracy(void) BANKED;

/**
 * Get the current accuracy as an integer percentage.
 *
 * Perfect = 100%
 * Early/Late = 50%
 * Miss = 0%
 *
 * @return  The accuracy percentage, from 0 to 100.
 */
uint8_t GetAccuracyPercent(void) BANKED;
