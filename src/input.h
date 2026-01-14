#pragma once

// high level input prompts

#include "cursor.h"
#include "board-state.h"
#include "board-gfx.h"
#include "indicator.h"
#include "move.h"

// prompts the user for a move
int8_t input_promptMoveStep(Cursor* cursor, BoardGFX* board, BoardState* state, Indicator* from, Indicator* to, Move* cache, int8_t cacheSize);
