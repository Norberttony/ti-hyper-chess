#include "indicator.h"
#include <graphx.h>
#include "gfx/gfx.h"

void indicator_draw(BoardGFX* board, Indicator* i)
{
    gfx_sprite_t* spr = 0;
    switch (i->type)
    {
        case Select:
            spr = select_ind;
            break;
        case Selected:
            spr = selected_ind;
            break;
        default:
            return;
    }
    int x = board->px + board->sqSize * i->sq.x;
    int y = board->py + board->sqSize * i->sq.y;
    gfx_TransparentSprite(spr, x, y);
}
