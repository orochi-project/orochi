#include "Banks/SetAutoBank.h"
#include "Print.h"
#include "Scroll.h"
// #include "SpriteManager.h"
#include "Text.h"
#include "ZGBMain.h"

IMPORT_MAP(menu_map);
IMPORT_TILES(japanese_glyphs);
IMPORT_TILES(yarara_font);

void START(void) {
    // scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
    InitScroll(BANK(menu_map), &menu_map, 0, 0);

    INIT_FONT(japanese_glyphs, PRINT_BKG);
    DrawText("ABEF", 5, 1, TEXT_ANCHOR_LEFT, 10, 2); // upper half of 大蛇
    DrawText("CDGH", 5, 2, TEXT_ANCHOR_LEFT, 10, 2); // lower half of 大蛇
    INIT_FONT(yarara_font, PRINT_BKG);
    DrawText("OROCHI", 9, 2, TEXT_ANCHOR_LEFT, 10, 2);
}

void UPDATE(void) {
    UpdateTypewriter();

    if (TypewriterIsDone(0)) {
    }
}

void DrawLogo(void) {}
