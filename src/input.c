#include "input.h"
#include <keypadc.h>
#include <graphx.h>
#include <time.h>
#include "keypad-extras.h"
#include "engine.h"
#include "gfx/gfx.h"
#include "defines.h"

void input_boardLoop(uint8_t isAgainstEngine, uint8_t engineSide)
{
    BoardState state = { 0 };
    state_init(&state);

    BoardGFX board =
    {
        .px = 4,
        .py = 4,
        .sqSize = 29,
        .lightIdx = 2,
        .darkIdx = 1,
        .isFlipped = 0
    };

    if (isAgainstEngine)
    {
        board.isFlipped = engineSide == white ? 1 : 0;
    }

    Cursor cursor = {
        .x = 200,
        .y = 200
    };
    cursor_init(&cursor, white_cursor);

    Move cache[MAX_MOVES];
    int8_t cacheSize = 0;

    Indicator from = { 0 };
    Indicator to = { 0 };

    int prev = clock();

    key_update();
    while (!kb_IsDown(kb_KeyClear))
    {
        if (state.toPlay == engineSide && isAgainstEngine && state.res == Result_Ongoing)
        {
            gfx_FillScreen(255);
            boardgfx_drawState(&board, &state);
            gfx_SwapDraw();

            // engine plays!
            SearchResult r = thinkForDepth(&state, 3);
            move_make(&state, r.bestMove);
            state.res = move_isGameOver(&state);
            prev = clock();
        }
        int curr = clock();
        float diff = (float)(curr - prev) / CLOCKS_PER_SEC;
        prev = curr;

        key_update();

        if (key_wasJustPressed(kb_Key2nd))
        {
            board.isFlipped = !board.isFlipped;
        }

        // perform state and graphical updates
        gfx_FillScreen(255);
        boardgfx_drawState(&board, &state);

        if (state.res == Result_Ongoing)
        {
            cacheSize = input_promptMoveStep(&cursor, &board, &state, &from, &to, cache, cacheSize);
        }

        // move and draw cursor
        cursor_readInput(&cursor, 150.0f * diff);
        cursor_draw(&cursor);

        gfx_SwapDraw();
    }
}

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
            state->res = move_isGameOver(state);
            from->type = Ind_Off;
            to->type = Ind_Off;
        }
    }
    else
    {
        indicator_drawMoves(board, cache, cacheSize, state->toPlay);
    }

    return cacheSize;
}
