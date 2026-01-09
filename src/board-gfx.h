#pragma once

#include "board-state.h"

typedef struct BoardGFX
{
    int px;
    int py;
    int sqSize;
    int lightIdx;
    int darkIdx;
} BoardGFX;

void drawBoardBG(BoardGFX* board);
void drawPiece(BoardGFX* board, int x, int y, int piece);
void drawBoardState(BoardGFX* board, BoardState* state);

static inline int get_piece_type(int piece)
{
    return piece & 0x7;
}

static inline int get_piece_side(int piece)
{
    return piece & 0x8;
}
