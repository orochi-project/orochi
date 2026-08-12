#include "GameData.h"
#include "GameAudio.h"
#include "Maps.h"
#include "Music.h"

uint8_t selected_map_idx;

bool buffer_allowed;

void PlayCurrentMapSong(void) {
    const Map *selected_map = &maps[selected_map_idx];
    audio_skip_interval = selected_map->audio_skip_interval;
    __PlayMusic((void *)selected_map->song, selected_map->song_bank, true);
}
