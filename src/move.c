#include "move.h"
#include <stdlib.h>
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
int8_t move_genRetractor(BoardState* state, Move* list, int8_t sq);
int8_t move_genImmobilizer(BoardState* state, Move* list, int8_t sq);
int8_t move_genCoordinator(BoardState* state, Move* list, int8_t sq);
int8_t move_genKing(BoardState* state, Move* list, int8_t sq);
int8_t move_genChameleon(BoardState* state, Move* list, int8_t sq);

static inline void set_piece_sq(BoardState* state, int8_t p, int8_t prevSq, int8_t sq, int8_t side)
{
    switch (get_piece_type(p))
    {
        case immobilizer:
            state->immSq[side_to_index(side)] = sq;
            return;
        case king:
            state->kingSq[side_to_index(side)] = sq;
            return;
        case coordinator:
            state->coordSq[side_to_index(side)] = sq;
            return;
        case chameleon:
            (void)side;
            uint8_t sti = side_to_cham_index(side);
            if (state->chamSq[sti] == prevSq)
            {
                state->chamSq[sti] = sq;
            }
            else
            {
                state->chamSq[sti + 1] = sq;
            }
            return;
        default:
            return;
    }
}

static inline int8_t is_adjacent(int8_t sq1, int8_t sq2)
{
    uint8_t v = abs(sq1 - sq2);
    return ((v >> 2) == 2 && (v & 0x3)) || v == 1;
}

void move_make(BoardState* state, Move* m)
{
    int8_t notToPlay = get_opposing_side(state->toPlay);

    // captures
    for (int8_t i = 0; i < m->captsCount; i++)
    {
        int sq = m->capts[i].sq;
        state->mailbox[sq] = 0;
        set_piece_sq(state, m->capts[i].piece, sq, -1, notToPlay);
    }

    // movement
    int8_t val = state->mailbox[m->from];
    state->mailbox[m->to] = val;
    state->mailbox[m->from] = 0;

    // update square
    set_piece_sq(state, val, m->from, m->to, state->toPlay);

    // toggle turn
    state->toPlay = notToPlay;
}

void move_unmake(BoardState* state, Move* m)
{
    int8_t notToPlay = state->toPlay;

    // toggle turn
    state->toPlay = get_opposing_side(state->toPlay);

    // unmovement
    int8_t val = state->mailbox[m->to];
    state->mailbox[m->from] = val;
    state->mailbox[m->to] = 0;

    set_piece_sq(state, val, m->to, m->from, state->toPlay);

    // uncaptures
    for (int8_t i = 0; i < m->captsCount; i++)
    {
        int8_t p = m->capts[i].piece;
        int8_t sq = m->capts[i].sq;
        set_piece_sq(state, p, -1, sq, notToPlay);
        state->mailbox[sq] = p;
    }
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
    if (
        val <= 0 ||
        get_piece_side(val) != state->toPlay ||
        is_adjacent(sq, state->immSq[side_to_index(get_opposing_side(state->toPlay))])
    )
    {
        return 0;
    }
    
    switch (get_piece_type(val))
    {
        case straddler:
            return move_genStraddler(state, list, sq);

        case springer:
            return move_genSpringer(state, list, sq);

        case retractor:
            return move_genRetractor(state, list, sq);

        case immobilizer:
            return move_genImmobilizer(state, list, sq);

        case coordinator:
            return move_genCoordinator(state, list, sq);

        case king:
            return move_genKing(state, list, sq);

        case chameleon:
            return move_genChameleon(state, list, sq);

        default:
            return 0;
    }
}

int8_t move_isLegal(BoardState* state, Move* move)
{
    int8_t side = state->toPlay;
    move_make(state, move);
    // can the opponent take our king?
    Move list[MAX_MOVES];
    int8_t size = move_gen(state, list);
    for (int8_t i = 0; i < size; i++)
    {
        Move* m = list + i;
        for (int8_t c = 0; c < m->captsCount; c++)
        {
            if (m->capts[c].piece == (side | king))
            {
                move_unmake(state, move);
                return 0;
            }
        }
    }
    move_unmake(state, move);
    return 1;
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
                int8_t* nextVal2 = state->mailbox + next2;
                if (
                    nextVal > 0 && get_piece_side(nextVal) != side && (
                        // normal straddler capture
                        *nextVal2 == (side | straddler) ||
                        // OR chameleon-straddler capture...
                        (*nextVal2 == (side | chameleon) && nextVal == (opp | straddler))
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
        int8_t mount = state->mailbox[idx];
        int8_t jumpIdx = idx + d;
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

int8_t move_genRetractor(BoardState* state, Move* list, int8_t sq)
{
    int8_t side = get_piece_side(state->mailbox[sq]);
    int8_t opp = get_opposing_side(side);
    int8_t size = 0;

    for (int8_t i = 0; i < 8; i++)
    {
        int8_t d = queenDirs[i];
        int8_t idx = sq + d;

        // determine if we can move backwards in order to capture a piece
        int8_t mount = state->mailbox[idx];
        int8_t jumpIdx = sq - d;
        int8_t jumpVal = state->mailbox[jumpIdx];
        if (jumpVal == 0)
        {
            Move* m = list + size++;
            
            m->from = sq;
            m->to = jumpIdx;

            if (mount >= 0 && get_piece_side(mount) == opp)
            {
                m->captsCount = 1;
                m->capts[0].piece = mount;
                m->capts[0].sq = idx;
                continue;
            }
            else
            {
                m->captsCount = 0;
            }
        }
        if (mount != 0)
        {
            continue;
        }
        idx += d;

        while (state->mailbox[idx] == 0)
        {
            Move* m = list + size++;

            m->from = sq;
            m->to = idx;
            m->captsCount = 0;

            idx += d;
        }
    }

    return size;
}

int8_t move_genImmobilizer(BoardState* state, Move* list, int8_t sq)
{
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
    }
    return size;
}

static inline void move_genDeathSquares(BoardState* state, Move* m, uint8_t ox, uint8_t oy, int8_t idx, int8_t filter)
{
    uint8_t opp = get_opposing_side(state->toPlay);
    uint8_t x = get_mailbox_x(idx);
    uint8_t y = get_mailbox_y(idx);

    int8_t dsqT[2] = {
        x + oy * MAILBOX_W,
        ox + y * MAILBOX_W
    };

    for (int8_t j = 0; j < 2; j++)
    {
        int8_t dsq = dsqT[j];
        int8_t dval = state->mailbox[dsq];
        if (dval > 0 && get_piece_side(dval) == opp && (!filter || filter == get_piece_type(dval)))
        {
            m->capts[m->captsCount].piece = dval;
            m->capts[m->captsCount].sq = dsq;
            m->captsCount++;
        }
    }
}

int8_t move_genCoordinator(BoardState* state, Move* list, int8_t sq)
{
    int8_t toPlay = state->toPlay;
    int8_t size = 0;

    int8_t kingSq = state->kingSq[side_to_index(toPlay)];
    uint8_t kingX = get_mailbox_x(kingSq);
    uint8_t kingY = get_mailbox_y(kingSq);

    int8_t cham1Sq = state->chamSq[side_to_cham_index(toPlay)];
    uint8_t cham1X = get_mailbox_x(cham1Sq);
    uint8_t cham1Y = get_mailbox_y(cham1Sq);

    int8_t cham2Sq = state->chamSq[side_to_cham_index(toPlay) + 1];
    uint8_t cham2X = get_mailbox_x(cham2Sq);
    uint8_t cham2Y = get_mailbox_y(cham2Sq);

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

            if (kingSq > -1)
            {
                move_genDeathSquares(state, m, kingX, kingY, idx, 0);
            }
            if (cham1Sq > -1)
            {
                move_genDeathSquares(state, m, cham1X, cham1Y, idx, king);
            }
            if (cham2Sq > -1)
            {
                move_genDeathSquares(state, m, cham2X, cham2Y, idx, king);
            }

            idx += d;
        }
    }
    return size;
}

int8_t move_genKing(BoardState* state, Move* list, int8_t sq)
{
    int8_t toPlay = state->toPlay;
    int8_t opp = get_opposing_side(toPlay);
    int8_t size = 0;

    int8_t coordSq = state->coordSq[side_to_index(toPlay)];
    uint8_t coordX = get_mailbox_x(coordSq);
    uint8_t coordY = get_mailbox_y(coordSq);

    int8_t cham1Sq = state->chamSq[side_to_cham_index(toPlay)];
    uint8_t cham1X = get_mailbox_x(cham1Sq);
    uint8_t cham1Y = get_mailbox_y(cham1Sq);

    int8_t cham2Sq = state->chamSq[side_to_cham_index(toPlay) + 1];
    uint8_t cham2X = get_mailbox_x(cham2Sq);
    uint8_t cham2Y = get_mailbox_y(cham2Sq);

    for (int8_t i = 0; i < 8; i++)
    {
        int8_t d = queenDirs[i];
        int8_t idx = sq + d;

        Move* m = 0;

        int8_t val = state->mailbox[idx];
        if (val == 0)
        {
            // move to empty square
            m = list + size++;
            m->from = sq;
            m->to = idx;
            m->captsCount = 0;
        }
        else if (val > 0 && get_piece_side(val) == opp)
        {
            // capture by displacement
            m = list + size++;
            m->from = sq;
            m->to = idx;
            m->capts[0].piece = val;
            m->capts[0].sq = idx;
            m->captsCount = 1;
        }
        else
        {
            continue;
        }

        // death squares with coordinator
        if (coordSq > -1)
        {
            move_genDeathSquares(state, m, coordX, coordY, idx, 0);
        }

        // death squares with chameleon, only against coordinator
        if (cham1Sq > -1)
        {
            move_genDeathSquares(state, m, cham1X, cham1Y, idx, coordinator);
        }
        if (cham2Sq > -1)
        {
            move_genDeathSquares(state, m, cham2X, cham2Y, idx, coordinator);
        }
    }

    return size;
}

int8_t move_genChameleon(BoardState* state, Move* list, int8_t sq)
{
    int8_t side = get_piece_side(state->mailbox[sq]);
    int8_t opp = get_opposing_side(side);
    int8_t size = 0;

    int8_t coordSq = state->coordSq[side_to_index(side)];
    uint8_t coordX = get_mailbox_x(coordSq);
    uint8_t coordY = get_mailbox_y(coordSq);

    int8_t kingSq = state->kingSq[side_to_index(side)];
    uint8_t kingX = get_mailbox_x(kingSq);
    uint8_t kingY = get_mailbox_y(kingSq);

    for (int8_t i = 0; i < 8; i++)
    {
        int8_t d = queenDirs[i];
        int8_t idx = sq + d;

        // to capture retractor must capture like retractor...
        int8_t mount = state->mailbox[idx];
        int8_t jumpIdx = sq - d;
        int8_t jumpVal = state->mailbox[jumpIdx];
        if (jumpVal == 0)
        {
            Move* m = list + size++;
            
            m->from = sq;
            m->to = jumpIdx;
            m->captsCount = 0;

            // to capture coordinator must capture like coordinator
            if (kingSq > -1)
            {
                move_genDeathSquares(state, m, kingX, kingY, jumpIdx, coordinator);
            }

            // to capture king must capture like king...
            if (coordSq > -1)
            {
                move_genDeathSquares(state, m, coordX, coordY, jumpIdx, king);
            }

            if (mount == (opp | retractor))
            {
                m->capts[m->captsCount].piece = mount;
                m->capts[m->captsCount].sq = idx;
                m->captsCount++;
                continue;
            }
        }
        else if (jumpVal == (opp | king))
        {
            // to capture king must capture like king...
            Move* m = list + size++;
            m->from = sq;
            m->to = jumpIdx;
            m->capts[0].piece = jumpVal;
            m->capts[0].sq = jumpIdx;
            m->captsCount = 1;
        }
        if (mount == 0)
        {
            idx += d;
        }
        
        while (state->mailbox[idx] == 0)
        {
            Move* m = list + size++;
            m->from = sq;
            m->to = idx;
            m->captsCount = 0;

            // to capture coordinator must capture like coordinator...
            if (coordSq > -1)
            {
                move_genDeathSquares(state, m, kingX, kingY, idx, coordinator);
            }

            // to capture straddler must capture like straddler...
            if ((i & 1) == 0)
            {
                for (int8_t c = 0; c < 4; c++)
                {
                    int8_t cd = rookDirs[c];
                    int8_t next = idx + cd;
                    int8_t next2 = idx + 2 * cd;
                    int8_t nextVal = state->mailbox[next];
                    int8_t* nextVal2 = state->mailbox + next2;
                    if (
                        nextVal > 0 && nextVal == (opp | straddler) && (
                            // normal chameleon capture
                            *nextVal2 == (side | straddler) ||
                            // OR chameleon-chameleon capture...
                            *nextVal2 == (side | chameleon)
                        )
                    )
                    {
                        m->capts[m->captsCount].piece = nextVal;
                        m->capts[m->captsCount].sq = next;
                        m->captsCount++;
                    }
                }
            }

            idx += d;
        }

        mount = state->mailbox[idx];
        jumpIdx = idx + d;
        if (mount == (opp | springer) && state->mailbox[jumpIdx] == 0)
        {
            // to capture springer must capture like springer...
            Move* m = list + size++;
            m->from = sq;
            m->to = jumpIdx;
            m->capts[0].piece = mount;
            m->capts[0].sq = idx;
            m->captsCount = 1;
        }
    }

    return size;
}
