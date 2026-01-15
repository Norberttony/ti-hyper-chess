#pragma once

#include "board-gfx.h"
#include "move.h"

enum IndicatorType
{
    Ind_Off,
    Ind_Select,
    Ind_Selected,
    Ind_WhiteMove,
    Ind_BlackMove
};

typedef struct Indicator
{
    enum IndicatorType type;
    Square sq;
} Indicator;

void indicator_draw(BoardGFX* board, Indicator* i);
void indicator_drawMoves(BoardGFX* board, Move* moves, uint8_t movesSize, uint8_t side);
