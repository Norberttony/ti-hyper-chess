#include "engine.h"
#include <debug.h>
#include "defines.h"

static Move bestMove;

int pieceValues[7] = {
    100,
    300,
    400,
    1000,
    1300,
    500,
    99999
};

int evaluate(BoardState* state)
{
    int score = 0;
    int8_t sti = side_to_index(state->toPlay);
    int8_t nsti = side_to_index(get_opposing_side(state->toPlay));

    // loop through all pieces and count up their material
    for (int8_t i = straddler; i <= king; i++)
    {
        score += pieceValues[i - 1] * state->pieceCounts[sti][i - 1];
        score -= pieceValues[i - 1] * state->pieceCounts[nsti][i - 1];
    }

    return score;
}

int think(BoardState* state, int alpha, int beta, int8_t depth, uint8_t isRoot)
{
    if (depth == 0)
    {
        return evaluate(state);
    }

    // generate moves...
    Move list[MAX_MOVES];
    int8_t size = move_gen(state, list);
    for (int8_t i = 0; i < size; i++)
    {
        Move* m = list + i;
        move_make(state, m);
        int score = -think(state, -beta, -alpha, depth - 1, 0);
        move_unmake(state, m);

        if (score >= beta)
        {
            return beta;
        }

        if (score > alpha)
        {
            alpha = score;

            if (isRoot)
            {
                bestMove = *m;
            }
        }
    }

    return alpha;
}

SearchResult thinkForDepth(BoardState* state, int8_t depth)
{
    SearchResult r;
    r.score = think(state, -999999, 999999, depth, 1);
    r.bestMove = &bestMove;
    dbg_printf("Score: %d\n", r.score);
    return r;
}
