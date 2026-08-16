#include "Banks/SetAutoBank.h"

#include "GameStore.h"
#include "Music.h"
#include "Print.h"
#include "Text.h"
#include "ZGBMain.h"
#include "gbdk/platform.h"

IMPORT_MAP(end_background);

IMPORT_TILES(yarara_font_primary);
IMPORT_TILES(yarara_font_secondary);

DECLARE_MUSIC(detour);

/** Represents the possible logo states. */
typedef enum {
    END_TITLE_TEXT,
    END_STATS_BOX,
    END_MAP_ID_LABEL,
    END_MAP_NAME_LABEL,
    END_PLAYER_ACCURACY_LABEL,
    END_PLAYER_GRADE_LABEL,
} EndCheckpoint;

/** Represents the stored font offsets for all loaded end-screen fonts. */
typedef struct {
    int8_t yarara_font_primary_font_offset;
    int8_t yarara_font_secondary_font_offset;
} FontOffset;

/** The current end-screen state. */
static EndCheckpoint end_checkpoint;

/** The saved game font offsets. */
static FontOffset font_offsets;

/** The title text typewriter index. */
static int8_t title_text_typewriter_idx;

/** Draw the end-screen title text. */
static void DrawTitleText(void);

void START(void) {
    RestoreDefaultAudio();

    InitScroll(BANK(end_background), &end_background, 0, 0);

    INIT_FONT(yarara_font_primary, PRINT_BKG);
    font_offsets.yarara_font_primary_font_offset = font_offset;

    INIT_FONT(yarara_font_secondary, PRINT_BKG);
    font_offsets.yarara_font_secondary_font_offset = font_offset;

    DrawTitleText();
}

void UPDATE(void) {
    UpdateTypewriter();

    // title done
    // now draw stats box
    if (end_checkpoint == END_TITLE_TEXT &&
        TypewriterIsDone(title_text_typewriter_idx)) {
        // TODO: Draw stats box.
        PlayMusic(detour,
                  1); // TODO: This should start after drawing the stats box.
        end_checkpoint = END_STATS_BOX;
    }
}

static void DrawTitleText(void) {
    wait_vbl_done();

    font_offset = font_offsets.yarara_font_primary_font_offset;

    static unsigned char title_label[] = "STATISTICS";

    DrawText(title_label, 10, 1, TEXT_ANCHOR_CENTER, 5,
             TEXT_PRIMARY_PALETTE_IDX);
}
