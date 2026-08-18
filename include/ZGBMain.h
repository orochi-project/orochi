#pragma once

#define STATES                                                                 \
    _STATE(StateMenu)                                                          \
    _STATE(StateGame)                                                          \
    _STATE(StateEnd)                                                           \
    STATE_DEF_END

#define SPRITES                                                                \
    _SPRITE(SpriteScanline, scanline, FLIP_NONE)                               \
    _SPRITE(SpriteTapNoteHorizontal, tap_note_horizontal, FLIP_NONE)           \
    _SPRITE(SpriteTapNoteVertical, tap_note_vertical, FLIP_NONE)               \
    _SPRITE(SpriteReverseNote, reverse_note, FLIP_NONE)                        \
    _SPRITE(SpriteHoldNote, hold_note, FLIP_NONE)                              \
    _SPRITE(SpriteLetterGrade, letter_grade, FLIP_NONE)                        \
    SPRITE_DEF_END

#include "ZGBMain_Init.h"
