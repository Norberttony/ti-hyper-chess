#include "board-state.h"
#include "defines.h"

void state_init(BoardState* state)
{
    for (int i = 0; i < 10; i++)
    {
        // (i, 0)
        state->mailbox[i] = -1;
        // (0, i)
        state->mailbox[i * 10] = -1;
        // (10, i)
        state->mailbox[i * 10 + 10] = -1;
        // (i, 10)
        state->mailbox[i + 90] = -1;
    }

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

    state->mailbox[a1] = white | immobilizer;
    state->mailbox[b1] = white | springer;
    state->mailbox[c1] = white | chameleon;
    state->mailbox[d1] = white | retractor;
    state->mailbox[e1] = white | king;
    state->mailbox[f1] = white | chameleon;
    state->mailbox[g1] = white | springer;
    state->mailbox[h1] = white | coordinator;
}
