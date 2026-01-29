#pragma once

#include "board-state.h"
#include "move.h"
#include "indicator.h"

typedef struct BoardGFX
{
    int px;
    int py;
    int sqSize;
    int lightIdx;
    int darkIdx;
    int isFlipped;
    // last move highlights
    Indicator fromHl;
    Indicator toHl;
} BoardGFX;

// draws the background of a board
void boardgfx_drawBg(BoardGFX* board);

// draws the background and the pieces of a board
void boardgfx_drawState(BoardGFX* board, BoardState* state);

// returns the square coordinates of pixel (x, y) graphically
Square boardgfx_pxToGfxSq(BoardGFX* board, int x, int y);

// turns a mailbox index into a gfx sq
Square boardgfx_stateSqToGfxSq(int sq);

// determines if the given square is out of bounds (assuming 8x8 board)
int boardgfx_isSqOutOfBounds(Square sq);

int boardgfx_areSquaresEqual(Square sq1, Square sq2);

// plays the move on the gfx and the state
void boardgfx_playMove(BoardGFX* board, BoardState* state, Move* move);

static inline void boardgfx_norm_sq(BoardGFX* board, Square* sq)
{
    if (board->isFlipped)
    {
        sq->x = BOARD_W - sq->x - 1;
        sq->y = BOARD_H - sq->y - 1;
    }
}
