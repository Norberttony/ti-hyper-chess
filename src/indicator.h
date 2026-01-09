#pragma once

#include "board-state.h"
#include "board-gfx.h"

enum IndicatorType
{
    Select,
    Selected,
    Move,
    Capture
};

typedef struct Indicator
{
    enum IndicatorType type;
    Square sq;
} Indicator;

void indicator_draw(BoardGFX* board, Indicator* i);
