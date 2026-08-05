#include "Banks/SetAutoBank.h"
#include "Print.h"
#include "Scroll.h"
// #include "SpriteManager.h"
#include "Text.h"
#include "ZGBMain.h"

IMPORT_MAP(menu_map);
IMPORT_TILES(japanese_glyphs);
IMPORT_TILES(yarara_font);

/** Represents the possible logo states. */
typedef enum { LOGO_KANJI, LOGO_ROMAJI } LogoState;

/** The current logo state. */
static LogoState logo_state;
/** The upper kanji typewriter index. */
static int8_t kanji_upper_typewriter_idx;
/** The lower kanji typewriter index. */
static int8_t kanji_lower_typewriter_idx;

void DrawKanji(void);
void DrawRomaji(void);

void START(void) {
    // scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
    InitScroll(BANK(menu_map), &menu_map, 0, 0);

    DrawKanji();
}

void UPDATE(void) {
    UpdateTypewriter();

    // kanji done
    // now draw romaji
    if (logo_state == LOGO_KANJI &&
        TypewriterIsDone(kanji_upper_typewriter_idx) &&
        TypewriterIsDone(kanji_lower_typewriter_idx)) {
        DrawRomaji();
    }
}

void DrawKanji(void) {
    INIT_FONT(japanese_glyphs, PRINT_BKG);

    kanji_upper_typewriter_idx =
        DrawText("ABEF", 4, 1, TEXT_ANCHOR_LEFT, 10, 2); // upper half of 大蛇
    kanji_lower_typewriter_idx =
        DrawText("CDGH", 4, 2, TEXT_ANCHOR_LEFT, 10, 2); // lower half of 大蛇

    logo_state = LOGO_KANJI;
}

void DrawRomaji(void) {
    INIT_FONT(yarara_font, PRINT_BKG);

    DrawText("OROCHI!", 9, 2, TEXT_ANCHOR_LEFT, 10, 2);

    logo_state = LOGO_ROMAJI;
}
