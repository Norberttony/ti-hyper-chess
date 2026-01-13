#include "move.h"
#include "defines.h"

const int8_t rookDirs[4] =
{
    1,      // right
    10,     // down
    -1,     // left
    -10     // up
};

const int8_t queenDirs[8] =
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

int8_t move_genStraddler(BoardState* state, Move* list, int8_t sq);
int8_t move_genSpringer(BoardState* state, Move* list, int8_t sq);

void move_make(BoardState* state, Move* m)
{
    // movement
    state->mailbox[m->to] = state->mailbox[m->from];
    state->mailbox[m->from] = 0;

    // captures
    for (int8_t i = 0; i < m->captsCount; i++)
    {
        state->mailbox[m->capts[i].sq] = 0;
    }

    // toggle turn
    state->toPlay = get_opposing_side(state->toPlay);
}

void move_unmake(BoardState* state, Move* m)
{
    // unmovement
    state->mailbox[m->from] = state->mailbox[m->to];
    state->mailbox[m->to] = 0;

    // uncaptures
    for (int8_t i = 0; i < m->captsCount; i++)
    {
        state->mailbox[m->capts[i].sq] = m->capts[i].piece;
    }

    // toggle turn
    state->toPlay = get_opposing_side(state->toPlay);
}

int8_t move_gen(BoardState* state, Move* list)
{
    int8_t size = 0;

    // go through each square...
    for (int8_t i = 0; i < MAILBOX_W * MAILBOX_H; i++)
    {
        int8_t val = state->mailbox[i];
        size += move_genPiece(state, list + size, i, val);
    }

    return size;
}

int8_t move_genPiece(BoardState* state, Move* list, int8_t sq, int8_t val)
{
    if (get_piece_side(val) != state->toPlay)
    {
        return 0;
    }
    switch (get_piece_type(val))
    {
        case straddler:
            return move_genStraddler(state, list, sq);

        case springer:
            return move_genSpringer(state, list, sq);

        default:
            return 0;
    }
}

int8_t move_genStraddler(BoardState* state, Move* list, int8_t sq)
{
    int8_t side = get_piece_side(state->mailbox[sq]);
    int8_t opp = get_opposing_side(side);
    int8_t size = 0;

    for (int8_t i = 0; i < 4; i++)
    {
        int8_t d = rookDirs[i];
        int8_t idx = sq + d;
        while (state->mailbox[idx] == 0)
        {
            Move* m = list + size++;

            m->from = sq;
            m->to = idx;
            m->captsCount = 0;

            // generate captures
            for (int8_t c = 0; c < 4; c++)
            {
                int8_t cd = rookDirs[c];
                int8_t next = idx + cd;
                int8_t next2 = idx + 2 * cd;
                int8_t nextVal = state->mailbox[next];
                int8_t nextVal2 = state->mailbox[next2];
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

int8_t move_genSpringer(BoardState* state, Move* list, int8_t sq)
{
    int8_t side = get_piece_side(state->mailbox[sq]);
    int8_t opp = get_opposing_side(side);
    int8_t size = 0;

    for (int8_t i = 0; i < 8; i++)
    {
        int8_t d = queenDirs[i];
        int8_t idx = sq + d;
        while (state->mailbox[idx] == 0)
        {
            Move* m = list + size++;

            m->from = sq;
            m->to = idx;
            m->captsCount = 0;

            idx += d;
        }

        // determine if we can jump over this piece
        int mount = state->mailbox[idx];
        int jumpIdx = idx + d;
        if (mount > 0 && get_piece_side(mount) == opp && state->mailbox[jumpIdx] == 0)
        {
            Move* m = list + size++;
            
            m->from = sq;
            m->to = jumpIdx;
            m->captsCount = 1;

            m->capts[0].piece = mount;
            m->capts[0].sq = idx;
        }
    }

    return size;
}
