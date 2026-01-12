#pragma once

// high level input prompts

#include "cursor.h"
#include "board-state.h"
#include "board-gfx.h"
#include "indicator.h"
#include "move-gen.h"

// prompts the user for a move
void input_promptMoveStep(Cursor* cursor, BoardGFX* board, Indicator* from, Indicator* to);
