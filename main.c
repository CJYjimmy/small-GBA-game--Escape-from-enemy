/*
 * Author: Jiayuan Chen
 */

#include "gba.h"
#include "logic.h"
#include "graphics.h"
#include "images/gbaTitle.h"
#include "images/win.h"
#include "images/lose.h"
#include "images/soldier.h"
#include "images/enemy.h"
#include "images/key.h"

#include <stdio.h>
#include <stdlib.h>

// AppState enum definition
typedef enum {
    // Add any states need for app.
    START,
    START_NODRAW,
    APP_INIT,
    APP_SHOW_LEVEL1,
    APP_PAUSE_LEVEL1,
    APP_LEVEL1,
    APP_LEVEL1_EXIT,
    APP_SHOW_LEVEL2,
    APP_PAUSE_LEVEL2,
    APP_LEVEL2,
    APP_EXIT,
    APP_EXIT_NODRAW,
} GBAState;

int main(void) {
    // Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    GBAState state = START;

    // We store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    // We store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    while(1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;

        // Manipulate the state machine below as needed.
        switch(state) {
        case START:
            // Wait for VBlank
            waitForVBlank();

            // Draw the start state here.
            drawFullScreenImageDMA((u16 *)gbaTitle);

            state = START_NODRAW;
            break;
        case START_NODRAW:
            // Check for a button press here to start the app.
            // Start the app by switching the state to APP_INIT.

            if (KEY_DOWN(BUTTON_START, currentButtons)) {
                state = APP_INIT;
            }

            break;
        case APP_INIT:
            // Initialize the app. Switch to the APP state.
            initializeAppState(&currentAppState);

            // Draw the initial state of the app
            fullDrawAppState(&currentAppState);

            state = APP_SHOW_LEVEL1;

            break;
        case APP_SHOW_LEVEL1:
            drawRectDMA(0, 60, 240, 40, WHITE);
            drawString(100, 70, "Level 1", BLACK);
            drawString(45, 85, "Press button A to start", BLACK);
            state = APP_PAUSE_LEVEL1;
            break;
        case APP_PAUSE_LEVEL1:
            if(KEY_DOWN(BUTTON_A, currentButtons)) {
                state = APP_LEVEL1;
                fullDrawAppState(&currentAppState);
            }
            if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                state = START;
            }
            break;
        case APP_LEVEL1:
            // Process the app for one frame, store the next state
            nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

            // Wait for VBlank before we do any drawing.
            waitForVBlank();

            // Undraw the previous state
            undrawAppState(&currentAppState);

            // Draw the current state
            drawAppState(&nextAppState);

            // Now set the current state as the next state for the next iter.
            currentAppState = nextAppState;

            // Check if the app is exiting. If it is, then go to the exit level1 state.
            if (nextAppState.gameOver) state = APP_EXIT;

            if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                state = START;
            }

            if (nextAppState.win) state = APP_LEVEL1_EXIT;

            break;
        case APP_LEVEL1_EXIT:
            initializeAppLevel2State(&currentAppState);
            fullDrawAppState(&currentAppState);
            state = APP_SHOW_LEVEL2;
            break;
        case APP_SHOW_LEVEL2:
            drawRectDMA(0, 60, 240, 40, WHITE);
            drawString(100, 70, "Level 2", BLACK);
            drawString(45, 85, "Press button A to start", BLACK);
            state = APP_PAUSE_LEVEL2;
            break;
        case APP_PAUSE_LEVEL2:
            if(KEY_DOWN(BUTTON_A, currentButtons)) {
                state = APP_LEVEL2;
                fullDrawAppState(&currentAppState);
            }
            if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                state = START;
            }
            break;
        case APP_LEVEL2:
            // Process the app for one frame, store the next state
            nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

            // Wait for VBlank before we do any drawing.
            waitForVBlank();

            // Undraw the previous state
            undrawAppState(&currentAppState);

            // Draw the current state
            drawAppState(&nextAppState);

            // Now set the current state as the next state for the next iter.
            currentAppState = nextAppState;

            if (nextAppState.gameOver) state = APP_EXIT;

            if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                state = START;
            }
            break;
        case APP_EXIT:
            // Wait for VBlank
            waitForVBlank();

            // Draw the exit / gameover screen
            if (currentAppState.win) {
                drawFullScreenImageDMA((u16 *)win);
            } else {
                drawFullScreenImageDMA((u16 *)lose);
            }

            state = APP_EXIT_NODRAW;
            break;
        case APP_EXIT_NODRAW:
            // Check for a button press here to go back to the start screen
            if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                state = START;
            }

            break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
