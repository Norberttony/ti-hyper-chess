#pragma once

#include "move.h"
#include "board-state.h"

typedef struct SearchResult
{
    int score;
    Move* bestMove;
} SearchResult;

SearchResult thinkForDepth(BoardState* state, int8_t depth);
