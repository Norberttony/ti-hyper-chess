#pragma once

#include "move.h"

// forward declaration from board-gfx
typedef struct BoardGFX BoardGFX;

enum IndicatorType
{
    Ind_Off,
    Ind_Select,
    Ind_Selected,
    Ind_WhiteMove,
    Ind_BlackMove,
    Ind_Highlight
};

typedef struct Square
{
    int x;
    int y;
} Square;

typedef struct Indicator
{
    enum IndicatorType type;
    Square sq;
} Indicator;

void indicator_draw(BoardGFX* board, Indicator* i);
void indicator_drawMoves(BoardGFX* board, Move* moves, uint8_t movesSize, uint8_t side);
