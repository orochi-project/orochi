#include "Banks/SetAutoBank.h"

#include "Scroll.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

IMPORT_MAP(map);

void START(void) {
    scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
    InitScroll(BANK(map), &map, 0, 0);
}

void UPDATE(void) {}
