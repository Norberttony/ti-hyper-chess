#include "cursor.h"
#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

void cursor_init(Cursor* cursor, gfx_sprite_t* sprite)
{
    cursor->sprite = sprite;
    cursor->bg = 0;
}

void cursor_readInput(Cursor* cursor, float speed)
{
    if (kb_IsDown(kb_KeyRight))
    {
        cursor->x += speed;
    }
    if (kb_IsDown(kb_KeyLeft))
    {
        cursor->x -= speed;
    }
    if (kb_IsDown(kb_KeyUp))
    {
        cursor->y -= speed;
    }
    if (kb_IsDown(kb_KeyDown))
    {
        cursor->y += speed;
    }
}

void cursor_draw(Cursor* cursor)
{
    if (!cursor->bg)
    {
        cursor->bg = gfx_MallocSprite(cursor->sprite->width, cursor->sprite->height);
    }
    int bgX = (int)cursor->x;
    int bgY = (int)cursor->y;
    cursor->bgX = bgX;
    cursor->bgY = bgY;
    gfx_GetSprite(cursor->bg, bgX, bgY);
    gfx_TransparentSprite(cursor->sprite, bgX, bgY);
}

void cursor_undraw(Cursor* cursor)
{
    if (!cursor->bg)
    {
        cursor->bg = gfx_MallocSprite(cursor->sprite->width, cursor->sprite->height);
    }
    else
    {
        gfx_Sprite(cursor->bg, (int)cursor->bgX, (int)cursor->bgY);
    }
}

void cursor_free(Cursor* cursor)
{
    if (cursor->bg)
    {
        free(cursor->bg);
    }
}
