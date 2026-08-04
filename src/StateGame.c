#include "Banks/SetAutoBank.h"

#include "Scroll.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

IMPORT_MAP(menu_map);

void START(void) {
    scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
    InitScroll(BANK(menu_map), &menu_map, 0, 0);
}

void UPDATE(void) {}
