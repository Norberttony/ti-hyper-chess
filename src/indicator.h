#pragma once

#include "board-state.h"
#include "board-gfx.h"
#include "move.h"

enum IndicatorType
{
    Ind_Off,
    Ind_Select,
    Ind_Selected,
    Ind_Move,
    Ind_Capture
};

typedef struct Indicator
{
    enum IndicatorType type;
    Square sq;
} Indicator;

void indicator_draw(BoardGFX* board, Indicator* i);
void indicator_drawMoves(BoardGFX* board, BoardState* state, Move* moves, int movesSize);
