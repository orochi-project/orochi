#include "Maps.h"
#include "MapDreamFlower.h"
#include "MapIsolation.h"
#include "MapKomorebi.h"
#include "Music.h"

DECLARE_MUSIC(komorebi);
DECLARE_MUSIC(dream_flower);
DECLARE_MUSIC(thirdboot);
DECLARE_MUSIC(isolation);

const Map maps[MAP_COUNT] = {
    {
        .id = 0,
        .name = "Komorebi",
        .difficulty = 1,
        .song = &komorebi_uge,
        .song_bank = (uint8_t)&__bank_komorebi_uge,
        .audio_skip_interval = 145,
        .notes = map_komorebi_notes,
        .note_count = MAP_KOMOREBI_NOTE_COUNT,
        .notes_bank = (uint8_t)&__bank_map_komorebi,
    },
    {
        .id = 1,
        .name = "Dream Flower",
        .difficulty = 2,
        .song = &dream_flower_uge,
        .song_bank = (uint8_t)&__bank_dream_flower_uge,
        .audio_skip_interval = 145,
        .notes = map_dream_flower_notes,
        .note_count = MAP_DREAM_FLOWER_NOTE_COUNT,
        .notes_bank = (uint8_t)&__bank_map_dream_flower,
    },
    {
        .id = 2,
        .name = "Thirdboot",
        .difficulty = 3,
        .song = &thirdboot_uge,
        .song_bank = (uint8_t)&__bank_thirdboot_uge,
        .audio_skip_interval = 145,
        .notes = map_dream_flower_notes,
        .note_count = MAP_DREAM_FLOWER_NOTE_COUNT,
        .notes_bank = (uint8_t)&__bank_map_dream_flower,
    },
    {
        .id = 3,
        .name = "Isolation",
        .difficulty = 4,
        .song = &isolation_uge,
        .song_bank = (uint8_t)&__bank_isolation_uge,
        .audio_skip_interval = 145,
        .notes = map_isolation_notes,
        .note_count = MAP_ISOLATION_NOTE_COUNT,
        .notes_bank = (uint8_t)&__bank_map_isolation,
    },
};

Note GetMapNote(const Note *notes, uint8_t notes_bank, uint16_t note_idx) {
    uint8_t _saved_bank = CURRENT_BANK;
    SWITCH_ROM(notes_bank);

    Note note = notes[note_idx];

    SWITCH_ROM(_saved_bank);

    return note;
}
