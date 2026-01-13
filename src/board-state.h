#pragma once

#include <stdint.h>

// dimensions of board
#define BOARD_W 8
#define BOARD_H 8

// dimensions of mailbox
#define MAILBOX_W 10
#define MAILBOX_H 10

// padding of mailbox
#define MAILBOX_PADW ((MAILBOX_W - BOARD_W) >> 1)
#define MAILBOX_PADH ((MAILBOX_H - BOARD_H) >> 1)

typedef struct BoardState
{
    // mailbox representation
    int8_t mailbox[MAILBOX_W * MAILBOX_H];
    int8_t toPlay;
    int8_t immSq[2];
} BoardState;

void state_init(BoardState* state);

// board -> mailbox conversion
static inline int board_to_mailbox(int x, int y)
{
    return x + MAILBOX_PADW + (y + MAILBOX_PADH) * MAILBOX_W;
}

static inline int8_t get_piece_type(int8_t piece)
{
    return piece & 0x7;
}

static inline int8_t get_piece_side(int8_t piece)
{
    return piece & 0x8;
}

static inline int8_t side_to_index(int8_t side)
{
    return side >> 3;
}
