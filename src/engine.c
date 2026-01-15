#include "engine.h"
#include <debug.h>
#include <time.h>
#include "defines.h"

static Move bestMove;
static int nodesVisited = 0;

int pieceValues[7] =
{
    100,
    300,
    400,
    1000,
    1300,
    500,
    0
};

void orderMoves(Move* list, uint8_t size)
{
    // move captures to the start
    int firstNonCaptIdx = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        Move* m = list + i;
        if (m->captsCount > 0)
        {
            if (i != firstNonCaptIdx)
            {
                // swap this capture with the first non-capture.
                Move temp = *m;
                *m = *(list + firstNonCaptIdx);
                *(list + firstNonCaptIdx) = temp;
            }
            firstNonCaptIdx++;
        }
    }
}

int evaluate(BoardState* state)
{
    int8_t persp = side_to_perspective(state->toPlay);
    int score = persp * state->materialScore;

    return score;
}

int thinkCaptures(BoardState* state, int alpha, int beta)
{
    nodesVisited++;
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

    orderMoves(list, size);

    for (int8_t i = 0; i < size; i++)
    {
        Move* m = list + i;
        int myVal = pieceValues[get_piece_type(state->mailbox[m->from]) - 1];
        int enemVal = pieceValues[get_piece_type(state->mailbox[m->capts[0].piece]) - 1];
        if (myVal > enemVal)
        {
            continue;
        }
        if (m->captsCount == 0)
        {
            break;
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
    if (state->kingSq[side_to_index(state->toPlay)] == -1)
    {
        return -99999;
    }
    if (depth == 0)
    {
        return thinkCaptures(state, alpha, beta);
    }
    nodesVisited++;

    // generate moves...
    Move list[MAX_MOVES];
    int8_t size = move_gen(state, list);

    orderMoves(list, size);

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
    nodesVisited = 0;
    SearchResult r;
    int prev = clock();
    r.score = think(state, -999999, 999999, depth, 1);
    int curr = clock();
    float time = (float)(curr - prev) / CLOCKS_PER_SEC;

    r.bestMove = &bestMove;
    r.nodesVisited = nodesVisited;
    r.timeTaken = time;

    dbg_printf("Score: %d\n", r.score);
    dbg_printf("Nodes visited: %d\n", r.nodesVisited);
    dbg_printf("Time taken: %f seconds\n", time);
    dbg_printf("NPS: %f\n", nodesVisited / time);
    dbg_printf("Best move: from %hhd to %hhd\n", bestMove.from, bestMove.to);
    return r;
}
