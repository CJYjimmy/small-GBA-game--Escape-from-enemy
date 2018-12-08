#include "logic.h"
#include "gba.h"

void initializeAppState(AppState* appState) {
    // Initialize everything that's part of this AppState struct here.
    appState->gameOver = 0;
    appState->win = 0;
    appState->level = 1;

    Soldier s = {134, 10, 134, 10};
    Key k = {10, 214, 10, 214, 0};
    Enemy e0 = {10, 26, 10, 26, 1};
    Enemy e1 = {10, 76, 10, 76, 1};
    Enemy e2 = {10, 165, 10, 159, 1};
    Boom b = {0, 0};
    AppState init = {0, 0, 1, s, {k, k}, {e0, e1, e2}, {b, b, b, b, b}};
    *appState = init;

}

void initializeAppLevel2State(AppState* appState) {
    // Initialize everything that's part of this AppState struct here.
    appState->gameOver = 0;
    appState->win = 0;
    appState->level = 2;

    Soldier s = {134, 10, 134, 10};
    Key k1 = {10, 214, 10, 214, 0};
    Key k2 = {130, 214, 130, 214, 0};
    Enemy e0 = {10, 26, 10, 26, 1};
    Enemy e1 = {10, 76, 10, 76, 1};
    Enemy e2 = {10, 165, 10, 159, 1};
    Boom b0 = {50,130};
    Boom b1 = {100,30};
    Boom b2 = {134,55};
    Boom b3 = {80,199};
    Boom b4 = {30,190};
    AppState init = {0, 0, 2, s, {k1, k2}, {e0, e1, e2}, {b0, b1, b2, b3, b4}};
    *appState = init;

}

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* Do all of app processing. This function gets called
     * every frame.
     */

    AppState nextAppState = *currentAppState;
    UNUSED(keysPressedBefore);

    // calculate the first enemy new position
    if (nextAppState.enemies[0].row < 135 && nextAppState.enemies[0].down == 1) {
        nextAppState.enemies[0].row++;
        nextAppState.enemies[0].col++;
        if (nextAppState.enemies[0].row == 135) {
            nextAppState.enemies[0].down = 0;
        }
    } else {
        nextAppState.enemies[0].row--;
        nextAppState.enemies[0].col--;
        if (nextAppState.enemies[0].row == 11) {
            nextAppState.enemies[0].down = 1;
        }
    }
    // calculate the second enemy new position
    if (nextAppState.enemies[1].row < 135 && nextAppState.enemies[1].down == 1) {
        nextAppState.enemies[1].row++;
        if (nextAppState.enemies[1].row == 135) {
            nextAppState.enemies[1].down = 0;
        }
    } else {
        nextAppState.enemies[1].row--;
        if (nextAppState.enemies[1].row == 11) {
            nextAppState.enemies[1].down = 1;
        }
    }
    if (nextAppState.enemies[1].row < 135 && nextAppState.enemies[1].down == 1) {
        nextAppState.enemies[1].row++;
        if (nextAppState.enemies[1].row == 135) {
            nextAppState.enemies[1].down = 0;
        }
    } else {
        nextAppState.enemies[1].row--;
        if (nextAppState.enemies[1].row == 11) {
            nextAppState.enemies[1].down = 1;
        }
    }
    // calculate the third enemy new position
    if (nextAppState.enemies[2].row < 135 && nextAppState.enemies[2].down == 1) {
        nextAppState.enemies[2].row++;
        if (nextAppState.enemies[2].row == 135) {
            nextAppState.enemies[2].down = 0;
        }
    } else {
        nextAppState.enemies[2].row--;
        if (nextAppState.enemies[2].row == 11) {
            nextAppState.enemies[2].down = 1;
        }
    }

    Soldier *s = &nextAppState.soldier;
    Soldier *currS = &currentAppState->soldier;

    if (nextAppState.level == 1) {
        // calculate the soilder new position according to the input button.
        if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
            if (s->col < 199 || (s->col <= 214 && s->row >= 10 && s->row <= 11)) {
                s->col++;
                s->pre_col++;
                currS->col++;
            }
        }

        if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
            if (s->col > 26 || (s->col > 10 && s->row >= 134 && s->row <= 135)) {
                s->col--;
                s->pre_col--;
                currS->col--;
            }
        }

        if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
            if (s->col >= 214 - 14 && s->row < 11) {
                s->row++;
                s->pre_row++;
                currS->row++;
            } else if (s->col >= 214 - 14) {
                // don't move
            } else if (s->row + 15 < 150) {
                s->row++;
                s->pre_row++;
                currS->row++;
            }
        }

        if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
            if (s->col <= 26 && s->row == 135) {
                s->row--;
                s->pre_row--;
                currS->row--;
            } else if (s->col <= 26) {
                // don't move
            }else if (s->row > 10) {
                s->row--;
                s->pre_row--;
                currS->row--;
            }
        }


        // to detect whether the soldier is touching enemies. IF so, player lose the
        // game and game over
        for (int i = 0; i < 3; i++) {
            int rowDiff = nextAppState.enemies[i].row - s->row;
            int colDiff = nextAppState.enemies[i].col - s->col;
            if (rowDiff > -15 && rowDiff < 15 && colDiff > -15 && colDiff < 15) {
                nextAppState.gameOver = 1;
            }
        }
        // to detect whether the soldier is touching key. If so, player win
        // and the game go to level 2
        int rowDiff_key = nextAppState.keys[0].row - s->row;
        int colDiff_key = nextAppState.keys[0].col - s->col;
        if (rowDiff_key > -15 && rowDiff_key < 15 && colDiff_key > -15 && colDiff_key < 15) {
            nextAppState.win = 1;
            nextAppState.level = 2;
        }
    } else if (nextAppState.level == 2) {
        // calculate the soilder new position according to the input button.
        if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
            if (s->col < 199 || (s->col <= 214 && s->row >= 10 && s->row <= 11) || (s->col <= 214 && s->row >= 130 && s->row <= 131)) {
                s->col++;
                s->pre_col++;
                currS->col++;
            }
        }

        if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
            if (s->col > 26 || (s->col > 10 && s->row >= 134 && s->row <= 135)) {
                s->col--;
                s->pre_col--;
                currS->col--;
            }
        }

        if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
            if ((s->col >= 214 - 14 && s->row < 11) || (s->col >= 214 - 14 && s->row < 131 && s->row >= 130)) {
                s->row++;
                s->pre_row++;
                currS->row++;
            } else if (s->col >= 214 - 14) {
                // don't move
            } else if (s->row + 15 < 150) {
                s->row++;
                s->pre_row++;
                currS->row++;
            }
        }

        if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
            if ((s->col <= 26 && s->row == 135) || (s->col >= 214 - 14 && s->row == 11)) {
                s->row--;
                s->pre_row--;
                currS->row--;
            } else if (s->col <= 26 || (s->col >= 214 - 14 && s->row < 131)) {
                // don't move
            } else if (s->row > 10) {
                s->row--;
                s->pre_row--;
                currS->row--;
            }
        }


        // to detect whether the soldier is touching enemies. IF so, player lose the
        // game and game over
        for (int i = 0; i < 3; i++) {
            int rowDiff = nextAppState.enemies[i].row - s->row;
            int colDiff = nextAppState.enemies[i].col - s->col;
            if (rowDiff > -15 && rowDiff < 15 && colDiff > -15 && colDiff < 15) {
                nextAppState.gameOver = 1;
            }
        }

        // to detect whether the soldier is touching the boom. If so, player lose the
        // game and game over
        for (int i = 0; i < 5; i++) {
            int rowDiff = nextAppState.booms[i].row - s->row;
            int colDiff = nextAppState.booms[i].col - s->col;
            if (rowDiff > -15 && rowDiff < 15 && colDiff > -15 && colDiff < 15) {
                nextAppState.gameOver = 1;
            }
        }

        // to detect whether the soldier is touching key. If so, player win the game
        // and the game over
        int count = 0;
        for (int i = 0; i < 2; i++) {
            int rowDiff_key = nextAppState.keys[i].row - s->row;
            int colDiff_key = nextAppState.keys[i].col - s->col;
            if (rowDiff_key > -15 && rowDiff_key < 15 && colDiff_key > -15 && colDiff_key < 15) {
                nextAppState.keys[i].obtained = 1;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (nextAppState.keys[i].obtained == 1) {
                count++;
            }
        }
        if (count == 2) {
            nextAppState.win = 1;
            nextAppState.gameOver = 1;
        }
    }


    return nextAppState;
}
