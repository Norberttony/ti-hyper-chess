#pragma once

#include "board-state.h";

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
} Move;

int gen_moves(BoardState* state, Move* list);

int gen_straddler(BoardState* state, Move* list, int sq);
