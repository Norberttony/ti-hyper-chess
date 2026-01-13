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

void move_make(BoardState* state, Move* m);
void move_unmake(BoardState* state, Move* m);

int move_gen(BoardState* state, Move* list);
int move_genPiece(BoardState* state, Move* list, int sq, int val);

int move_genStraddler(BoardState* state, Move* list, int sq);
