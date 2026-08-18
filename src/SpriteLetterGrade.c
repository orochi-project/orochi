#include "Banks/SetAutoBank.h"

#include "GameData.h"
#include "Sprite.h"

void START(void) {}

void UPDATE(void) {}

void DESTROY(void) {}

/** Redraw the letter grade frame. */
void RefreshLetterGrade(Sprite *sprite) BANKED {
    LetterGradeData *letter_grade_data = (LetterGradeData *)sprite->custom_data;

    switch (letter_grade_data->letter) {
    case 'M': // max
        SetFrame(sprite, 0);
        break;
    case 'S':
        SetFrame(sprite, 1);
        break;
    case 'A':
        SetFrame(sprite, 2);
        break;
    case 'B':
        SetFrame(sprite, 3);
        break;
    case 'C':
        SetFrame(sprite, 4);
        break;
    case 'D':
    default:
        SetFrame(sprite, 5);
        break;
    }
}
