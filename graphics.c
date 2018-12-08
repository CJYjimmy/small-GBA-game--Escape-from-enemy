#include "graphics.h"
#include "gba.h"
#include "images/gbaTitle.h"
#include "images/win.h"
#include "images/soldier.h"
#include "images/enemy.h"
#include "images/key.h"
#include "images/boom.h"

// Add any draw/undraw functions for sub-elements of app.

static void drawSoldier(Soldier *s){
    drawImageDMA(s->col, s->row, 15, 15, (u16 *) soldier);
}

static void drawKey(Key* k){
    drawImageDMA(k->col, k->row, 15, 15, (u16 *) key);
}

static void drawEnemy(Enemy *e){
    drawImageDMA(e->col, e->row, 15, 15, (u16 *) enemy);
}

static void drawBoom(Boom *b){
    drawImageDMA(b->col, b->row, 15, 15, (u16 *) boom);
}

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    // draw the background
    fillScreenDMA(GREEN);
    // draw the area which the "soldier can move"
    drawRectDMA(10, 134, 16, 16, BLACK);
    drawRectDMA(26, 10, 188, 140, BLACK);
    drawRectDMA(214, 10, 16, 16, BLACK);
    // draw all game element
    drawSoldier(&(state->soldier));
    drawKey(&(state->keys[0]));
    for (int i = 0; i < 3; i++) {
        drawEnemy(&(state->enemies[i]));
    }

    if (state->level == 2) {
        drawRectDMA(214, 130, 16, 16, BLACK);
        drawKey(&(state->keys[1]));
        for (int i = 0; i < 5; i++) {
            drawBoom(&(state->booms[i]));
        }
    }

}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    Soldier s = state->soldier;
    Enemy *enemies = state->enemies;
    Key *keys = state->keys;
    // erase the previous soldier
    if (s.pre_row != s.row || s.pre_col != s.col) {
        drawRectDMA(s.col, s.row, 15, 15, BLACK);
        drawRectDMA(s.pre_col, s.pre_row, 15, 15, BLACK);
    }
    // erase the previous enemy
    for (int i = 0; i < 3; i++) {
        drawRectDMA(enemies[i].col, enemies[i].row, 15, 15, BLACK);
    }
    // erase the key
    for (int i = 0; i < state->level; i++) {
        if (keys[i].obtained == 1) {
            drawRectDMA(keys[i].col, keys[i].row, 15, 15, BLACK);
        }
    }

}

// This function will be used to draw things that might have moved in a frame.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    Soldier s = state->soldier;
    drawSoldier(&s);
    for (int i = 0; i < 3; i++) {
        drawEnemy(&(state->enemies[i]));
    }
}
