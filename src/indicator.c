#include "indicator.h"
#include <graphx.h>
#include "gfx/gfx.h"
#include "defines.h"
#include "board-gfx.h"

void indicator_draw(BoardGFX* board, Indicator* i)
{
    // convert to location of square graphically
    Square loc = i->sq;
    boardgfx_norm_sq(board, &loc);

    gfx_sprite_t* spr = 0;
    int sqSize = board->sqSize;
    int x = board->px + sqSize * loc.x;
    int y = board->py + sqSize * loc.y;
    switch (i->type)
    {
        case Ind_Select:
            spr = select_ind;
            break;
        case Ind_Selected:
            spr = selected_ind;
            break;
        case Ind_WhiteMove:
            gfx_SetColor(3);
            goto draw_move;
        case Ind_BlackMove:
            gfx_SetColor(4);
        draw_move:
            (void)x;
            int s = 7; // size of move indicator
            int pad = (sqSize - s) / 2;
            gfx_FillRectangle(x + pad, y + pad, s, s);
            return;
        case Ind_Highlight:
            gfx_SetColor(119);
            gfx_FillRectangle(x, y, sqSize, sqSize);
            return;
        default:
            return;
    }
    gfx_TransparentSprite(spr, x, y);
}

void indicator_drawMoves(BoardGFX* board, Move* moves, uint8_t movesSize, uint8_t side)
{
    Indicator ind = { 0 };
    ind.type = side == white ? Ind_WhiteMove : Ind_BlackMove;

    for (uint8_t i = 0; i < movesSize; i++)
    {
        ind.sq = boardgfx_stateSqToGfxSq(moves[i].to);
        indicator_draw(board, &ind);
    }
}
