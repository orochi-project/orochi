#include "GameData.h"
#include "Maps.h"
#include "Music.h"
#include "stdbool.h"
#include <stdint.h>

uint8_t selected_map_idx;

void PlayCurrentMapSong(void) {
    const Map *selected_map = &maps[selected_map_idx];
    __PlayMusic((void *)selected_map->song, selected_map->song_bank, true);
}
