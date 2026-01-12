#pragma once

#include "board-state.h"

#define MAX_MOVES 128

typedef struct PieceCapture
{
    int piece;
    int sq;
} PieceCapture;

typedef struct Move
{
    // from and to are mailbox indices
    int from;
    int to;
    PieceCapture capts[4];
    int captsCount;
} Move;

int gen_moves(BoardState* state, Move* list);
int gen_pieceMoves(BoardState* state, Move* list, int sq, int val);

int gen_straddler(BoardState* state, Move* list, int sq);
