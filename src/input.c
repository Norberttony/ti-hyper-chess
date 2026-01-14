#include "input.h"
#include <keypadc.h>
#include "keypad-extras.h"
#include "move.h"

int8_t input_promptMoveStep(Cursor* cursor, BoardGFX* board, BoardState* state, Indicator* from, Indicator* to, Move* cache, int8_t cacheSize)
{
    // determine which indicator user is deciding for...
    Indicator* active = 0;
    Indicator* prev = 0;
    if (from->type == Ind_Off || from->type == Ind_Select)
    {
        active = from;
    }
    else
    {
        prev = from;
        active = to;
    }

    if (prev)
    {
        indicator_draw(board, prev);
    }

    active->type = Ind_Select;

    Square sq = boardgfx_pxToGfxSq(board, cursor->x, cursor->y);
    if (!boardgfx_isSqOutOfBounds(sq) && active)
    {
        active->sq = sq;
        if (!prev || !boardgfx_areSquaresEqual(prev->sq, sq))
        {
            indicator_draw(board, active);
        }

        if (key_wasJustPressed(kb_KeyEnter))
        {
            if (prev && boardgfx_areSquaresEqual(prev->sq, active->sq))
            {
                // deselect
                prev->type = Ind_Off;
            }
            else
            {
                // select
                active->type = Ind_Selected;
            }
        }
    }

    if (from->type == Ind_Selected)
    {
        // was "from" just selected?
        if (active == from)
        {
            // update moves only when necessary
            int mSq = board_to_mailbox(from->sq.x, from->sq.y);
            cacheSize = move_genPiece(state, cache, mSq, state->mailbox[mSq]);
            cacheSize = move_filterIllegal(state, cache, cacheSize);
        }
    }
    else
    {
        cacheSize = 0;
    }

    if (to->type == Ind_Selected)
    {
        int toSq = board_to_mailbox(to->sq.x, to->sq.y);

        // check if this move exists and make it
        Move* chosen = 0;
        for (int i = 0; i < cacheSize; i++)
        {
            Move* m = cache + i;
            if (m->to == toSq)
            {
                chosen = m;
                break;
            }
        }

        if (chosen)
        {
            move_make(state, chosen);
            from->type = Ind_Off;
            to->type = Ind_Off;
        }
    }
    else
    {
        indicator_drawMoves(board, cache, cacheSize);
    }

    return cacheSize;
}
