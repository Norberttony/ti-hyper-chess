#pragma once

#include <graphx.h>

typedef struct Cursor
{
    float x;
    float y;
    gfx_sprite_t* sprite;
    int bgX;
    int bgY;
    gfx_sprite_t* bg;
} Cursor;

void cursor_init(Cursor* cursor, gfx_sprite_t* sprite);
void cursor_readInput(Cursor* cursor, float speed);
void cursor_draw(Cursor* cursor);
void cursor_undraw(Cursor* cursor);
