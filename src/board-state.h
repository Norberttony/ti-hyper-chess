#pragma once

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
    int mailbox[MAILBOX_W * MAILBOX_H];
    int toPlay;
} BoardState;

void state_init(BoardState* state);

// board -> mailbox conversion
static inline int board_to_mailbox(int x, int y)
{
    return x + MAILBOX_PADW + (y + MAILBOX_PADH) * MAILBOX_W;
}

static inline int get_piece_type(int piece)
{
    return piece & 0x7;
}

static inline int get_piece_side(int piece)
{
    return piece & 0x8;
}
