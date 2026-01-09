#pragma once

typedef struct BoardGFX
{
    int px;
    int py;
    int sqSize;
} BoardGFX;

void drawBoardBG(BoardGFX* board, int darkIdx, int lightIdx);
void drawPiece(BoardGFX* board, int x, int y, int piece);

static inline int get_piece_type(int piece)
{
    return piece & 0x7;
}

static inline int get_piece_side(int piece)
{
    return piece & 0x8;
}
