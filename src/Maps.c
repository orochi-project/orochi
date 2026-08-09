#include "Maps.h"
#include "Music.h"

DECLARE_MUSIC(komorebi);
DECLARE_MUSIC(dream_flower);
DECLARE_MUSIC(thirdboot);
DECLARE_MUSIC(isolation);

const Map maps[MAP_COUNT] = {
    {.id = 0,
     .name = "Komorebi",
     .difficulty = 1,
     .song = &komorebi_uge,
     .song_bank = (uint8_t)&__bank_komorebi_uge},
    {.id = 1,
     .name = "Dream Flower",
     .difficulty = 2,
     .song = &dream_flower_uge,
     .song_bank = (uint8_t)&__bank_dream_flower_uge},
    {.id = 2,
     .name = "Thirdboot",
     .difficulty = 3,
     .song = &thirdboot_uge,
     .song_bank = (uint8_t)&__bank_thirdboot_uge},
    {.id = 3,
     .name = "Isolation",
     .difficulty = 4,
     .song = &isolation_uge,
     .song_bank = (uint8_t)&__bank_isolation_uge},
};
