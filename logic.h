#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

/*
* Add any additional structs that I need for app.
*
*/

typedef struct {
    int row;
    int col;
    int pre_row;
    int pre_col;
} Soldier;

typedef struct
{
    int row;
    int col;
    int pre_row;
    int pre_col;
    // whether the key is obtained by soldier
    int obtained;
} Key;

typedef struct
{
    int row;
    int col;
    int pre_row;
    int pre_col;
    int down;
} Enemy;

typedef struct
{
    int row;
    int col;
} Boom;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int win;
    int level;

    Soldier soldier;
    Key keys[2];
    Enemy enemies[3];
    Boom booms[5];

} AppState;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

void initializeAppLevel2State(AppState* appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
