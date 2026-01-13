#include "board-state.h"
#include "defines.h"

void state_init(BoardState* state)
{
    // initialize out of bounds squares
    for (int y = 0; y < MAILBOX_PADH; y++)
    {
        for (int x = 0; x < MAILBOX_W; x++)
        {
            // top
            state->mailbox[x + y * MAILBOX_W] = -1;
            // bottom
            state->mailbox[x + (MAILBOX_H - y - 1) * MAILBOX_W] = -1;
        }
    }
    for (int x = 0; x < MAILBOX_PADW; x++)
    {
        for (int y = 0; y < MAILBOX_H; y++)
        {
            // top
            state->mailbox[x + y * MAILBOX_W] = -1;
            // bottom
            state->mailbox[MAILBOX_W - 1 - x + y * MAILBOX_W] = -1;
        }
    }

    // hard-coded start position
    state->toPlay = white;

    // straddlers
    for (int i = a2; i <= h2; i++)
    {
        state->mailbox[i] = white | straddler;
    }
    for (int i = a7; i <= h7; i++)
    {
        state->mailbox[i] = black | straddler;
    }

    // rest of the pieces
    state->mailbox[a8] = black | immobilizer;
    state->mailbox[b8] = black | springer;
    state->mailbox[c8] = black | chameleon;
    state->mailbox[d8] = black | retractor;
    state->mailbox[e8] = black | king;
    state->mailbox[f8] = black | chameleon;
    state->mailbox[g8] = black | springer;
    state->mailbox[h8] = black | coordinator;

    state->mailbox[a1] = white | coordinator;
    state->mailbox[b1] = white | springer;
    state->mailbox[c1] = white | chameleon;
    state->mailbox[d1] = white | retractor;
    state->mailbox[e1] = white | king;
    state->mailbox[f1] = white | chameleon;
    state->mailbox[g1] = white | springer;
    state->mailbox[h1] = white | immobilizer;

    // quick look ups
    state->immSq[0] = h1;
    state->immSq[1] = a8;
    state->kingSq[0] = e1;
    state->kingSq[1] = e8;
    state->coordSq[0] = a1;
    state->coordSq[1] = h8;
}
