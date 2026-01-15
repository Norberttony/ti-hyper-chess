#include "engine.h"
#include <debug.h>
#include "defines.h"

static Move bestMove;

int pieceValues[7] =
{
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
    int8_t persp = side_to_perspective(state->toPlay);
    int score = persp * state->materialScore;

    return score;
}

int thinkCaptures(BoardState* state, int alpha, int beta)
{
    int score = evaluate(state);
    if (score >= beta)
    {
        return beta;
    }
    if (score > alpha)
    {
        alpha = score;
    }

    Move list[MAX_MOVES];
    int8_t size = move_gen(state, list);
    for (int8_t i = 0; i < size; i++)
    {
        Move* m = list + i;
        if (m->captsCount == 0)
        {
            continue;
        }

        move_make(state, m);
        score = -thinkCaptures(state, -beta, -alpha);
        move_unmake(state, m);

        if (score >= beta)
        {
            return beta;
        }

        if (score > alpha)
        {
            alpha = score;
        }
    }

    return alpha;
}

int think(BoardState* state, int alpha, int beta, int8_t depth, uint8_t isRoot)
{
    if (depth == 0)
    {
        return thinkCaptures(state, alpha, beta);
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
    dbg_printf("Best move: from %hhd to %hhd\n", bestMove.from, bestMove.to);
    return r;
}
