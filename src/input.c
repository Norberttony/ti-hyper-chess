#include "input.h"
#include <keypadc.h>
#include "keypad-extras.h"
#include "move.h"

void input_promptMoveStep(Cursor* cursor, BoardGFX* board, BoardState* state, Indicator* from, Indicator* to)
{
    Move moveList[MAX_MOVES];
    int moveListSize = 0;

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

    active->type = Ind_Select;

    Square sq = boardgfx_pxToGfxSq(board, cursor->x, cursor->y);
    if (!boardgfx_isSqOutOfBounds(sq) && active)
    {
        active->sq = sq;
        if (!prev || !boardgfx_areSquaresEqual(prev->sq, sq))
        {
            indicator_draw(board, active);
        }

        if (prev)
        {
            indicator_draw(board, prev);
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
        int mSq = board_to_mailbox(from->sq.x, from->sq.y);
        // update moves...
        moveListSize = move_genPiece(state, moveList, mSq, state->mailbox[mSq]);
    }
    else
    {
        moveListSize = 0;
    }

    indicator_drawMoves(board, moveList, moveListSize);
}
