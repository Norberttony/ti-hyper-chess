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
        .px = 44,
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
        .y = 100
    };
    cursor_init(&cursor, white_cursor);

    Move* cache = (Move*)malloc(MAX_MOVES * sizeof(Move));
    uint8_t cacheSize = 0;

    Indicator from = { 0 };
    Indicator to = { 0 };

    int prev = clock();

    while (1)
    {
        int curr = clock();
        float diff = (float)(curr - prev) / CLOCKS_PER_SEC;
        prev = curr;

        // draw the entire screen (excluding cursor...)
        gfx_FillScreen(5);
        boardgfx_drawState(&board, &state);

        // draw an X button
        gfx_SetColor(194);
        gfx_SetTextBGColor(194);
        gfx_SetTextScale(2, 2);
        gfx_FillRectangle(295, 4, 20, 20);
        gfx_SetTextXY(298, 7);
        gfx_PrintString("X");

        if (state.toPlay == engineSide && isAgainstEngine && state.res == Result_Ongoing)
        {
            // handle engine input...
            gfx_SwapDraw();

            // engine plays!
            SearchResult r = thinkForDepth(&state, 3);
            boardgfx_playMove(&board, &state, r.bestMove);
            state.res = move_isGameOver(&state);

            // reset frame counter
            prev = clock();
        }
        else
        {
            // handle user input...
            key_update();

            if (key_wasJustPressed(kb_Key2nd))
            {
                board.isFlipped = !board.isFlipped;
            }
            else if (key_wasJustReleased(kb_KeyEnter) && cursor.x >= 290 && cursor.y <= 25)
            {
                // hit the X button
                break;
            }

            if (state.res == Result_Ongoing)
            {
                cacheSize = input_promptMoveStep(&cursor, &board, &state, &from, &to, cache, cacheSize);
                if (!isAgainstEngine && to.type == Ind_Off && from.type == Ind_Off)
                {
                    // flip the board to match whose turn it is
                    board.isFlipped = state.toPlay == white ? 0 : 1;
                }
            }
    
            // move and draw cursor
            cursor_readInput(&cursor, 150.0f * diff);
            cursor_draw(&cursor);
    
            gfx_SwapDraw();
        }
    }

    free(cache);
}

int8_t input_promptMoveStep(Cursor* cursor, BoardGFX* board, BoardState* state, Indicator* from, Indicator* to, Move* cache, int8_t cacheSize)
{
    if (from->type == Ind_Off)
    {
        from->type = Ind_Select;
    }

    Square onSq = boardgfx_pxToGfxSq(board, cursor->x, cursor->y);
    int mSq = 0;
    int8_t val = -1;
    uint8_t hasFromBeenSelectedNow = 0;

    Indicator* active = from->type == Ind_Select ? from : to;

    if (!boardgfx_isSqOutOfBounds(onSq))
    {
        mSq = board_to_mailbox(onSq.x, onSq.y);
        val = state->mailbox[mSq];
        active->sq = onSq;
        active->type = Ind_Select;
    }
    else
    {
        active->type = Ind_Off;
    }

    // has a piece been selected?
    if (val > 0 && get_piece_side(val) == state->toPlay && key_wasJustPressed(kb_KeyEnter))
    {
        if (from->type == Ind_Selected && boardgfx_areSquaresEqual(from->sq, onSq))
        {
            // deselect
            from->type = Ind_Select;
            to->type = Ind_Off;
            cacheSize = 0;
        }
        else
        {
            // select
            from->sq = onSq;
            from->type = Ind_Selected;
            cacheSize = move_genPiece(state, cache, mSq, val);
            cacheSize = move_filterIllegal(state, cache, cacheSize);
            hasFromBeenSelectedNow = 1;
        }
    }

    if (val > -1 && from->type == Ind_Selected && !hasFromBeenSelectedNow && key_wasJustPressed(kb_KeyEnter))
    {
        // consider the input as an attempt to select "to"
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
            boardgfx_playMove(board, state, chosen);
            state->res = move_isGameOver(state);
            from->type = Ind_Off;
            to->type = Ind_Off;
            cacheSize = 0;
        }
    }
    else
    {
        indicator_drawMoves(board, cache, cacheSize, state->toPlay);
    }

    indicator_draw(board, from);
    indicator_draw(board, to);

    return cacheSize;
}
