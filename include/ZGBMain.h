#pragma once

#define STATES                                                                 \
    _STATE(StateMenu)                                                          \
    STATE_DEF_END

#define SPRITES                                                                \
    _SPRITE(SpritePlayer, player, FLIP_NONE)                                   \
    SPRITE_DEF_END

#include "ZGBMain_Init.h"
