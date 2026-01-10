#pragma once

#include "board-state.h"

typedef struct BoardGFX
{
    int px;
    int py;
    int sqSize;
    int lightIdx;
    int darkIdx;
    int isFlipped;
} BoardGFX;

typedef struct Square
{
    int x;
    int y;
} Square;

// draws the background of a board
void boardgfx_drawBg(BoardGFX* board);

// draws the background and the pieces of a board
void boardgfx_drawState(BoardGFX* board, BoardState* state);

// returns the square coordinates of pixel (x, y) graphically
Square boardgfx_getGfxSq(BoardGFX* board, int x, int y);

// determines if the given square is out of bounds (assuming 8x8 board)
int boardgfx_isSqOutOfBounds(Square sq);

int boardgfx_areSquaresEqual(Square sq1, Square sq2);

static inline int get_piece_type(int piece)
{
    return piece & 0x7;
}

static inline int get_piece_side(int piece)
{
    return piece & 0x8;
}
