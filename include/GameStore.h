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
