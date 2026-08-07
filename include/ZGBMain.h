#pragma once

#define STATES                                                                 \
    _STATE(StateMenu)                                                          \
    _STATE(StateGame)                                                          \
    STATE_DEF_END

#define SPRITES                                                                \
    _SPRITE(SpritePlayer, player, FLIP_NONE)                                   \
    _SPRITE(SpriteScanline, scanline, FLIP_NONE)                               \
    _SPRITE(SpriteTapNoteHorizontal, tap_note_horizontal, FLIP_NONE)           \
    _SPRITE(SpriteTapNoteVertical, tap_note_vertical, FLIP_NONE)               \
    SPRITE_DEF_END

#include "ZGBMain_Init.h"

extern Sprite *scanline_sprite;
