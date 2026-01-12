#include "indicator.h"
#include <graphx.h>
#include "gfx/gfx.h"

void indicator_draw(BoardGFX* board, Indicator* i)
{
    gfx_sprite_t* spr = 0;
    int x = board->px + board->sqSize * i->sq.x;
    int y = board->py + board->sqSize * i->sq.y;
    switch (i->type)
    {
        case Ind_Select:
            spr = select_ind;
            break;
        case Ind_Selected:
            spr = selected_ind;
            break;
        case Ind_Move:
            gfx_SetColor(0);
            int s = 5; // size of move indicator
            int sqSize = board->sqSize;
            int pad = (sqSize - s) / 2;
            gfx_FillRectangle(x + pad, y + pad, s, s);
            return;
        default:
            return;
    }
    gfx_TransparentSprite(spr, x, y);
}

void indicator_drawMoves(BoardGFX* board, Move* moves, int movesSize)
{
    Indicator ind = { 0 };
    ind.type = Ind_Move;

    for (int i = 0; i < movesSize; i++)
    {
        ind.sq = boardgfx_stateSqToGfxSq(board, moves[i].to);
        indicator_draw(board, &ind);
    }
}
