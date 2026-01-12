#include "move-gen.h"
#include "defines.h"

const int rookDirs[4] =
{
    1,      // right
    10,     // down
    -1,     // left
    -10     // up
};

const int queenDirs[8] =
{
    1,      // right
    11,     // down right
    10,     // down
    9,      // down left
    -1,     // left
    -11,    // up left
    -10,    // up
    -9      // up right
};

int genMoves(BoardState* state, Move* list)
{
    int size = 0;

    // go through each square...
    for (int i = 0; i < MAILBOX_W * MAILBOX_H; i++)
    {
        int val = state->mailbox[i];
        switch (get_piece_type(val))
        {
            // ignore empty and invalid squares
            case 0:
            case -1:
                continue;
            
            case straddler:
                size += gen_straddler(state, list, i);
                break;
        }
    }

    return size;
}

int gen_straddler(BoardState* state, Move* list, int sq)
{
    int side = get_piece_side(state->mailbox[sq]);
    int opp = get_opposing_side(side);
    int size = 0;

    for (int i = 0; i < 4; i++)
    {
        int d = rookDirs[i];
        int idx = sq + d;
        while (state->mailbox[idx] == 0)
        {
            Move* m = list + size++;

            m->from = sq;
            m->to = idx;
            m->captsCount = 0;

            // generate captures
            for (int c = 0; c < 4; c++)
            {
                int cd = rookDirs[c];
                int next = idx + cd;
                int next2 = idx + 2 * cd;
                int nextVal = state->mailbox[next];
                int nextVal2 = state->mailbox[next2];
                if (
                    nextVal > 0 && get_piece_type(nextVal) != side && (
                        // normal straddler capture
                        nextVal2 == (side | straddler) ||
                        // OR chameleon-straddler capture...
                        (nextVal2 == (side | chameleon) && nextVal == (opp | straddler))
                    )
                )
                {
                    m->capts[m->captsCount].piece = nextVal;
                    m->capts[m->captsCount].sq = next;
                    m->captsCount++;
                }
            }
            idx += d;
        }
    }

    return size;
}
