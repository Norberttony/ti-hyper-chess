#pragma once

typedef struct BoardState
{
    // mailbox representation
    int mailbox[100];
} BoardState;

void state_init(BoardState* state);

// 8x8 board -> 10x10 board conversion
static inline int eightToTen(int x, int y)
{
    return x + 1 + (y + 1) * 10;
}
