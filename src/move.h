#pragma once

#include "board-state.h"

#define MAX_MOVES 128

typedef struct PieceCapture
{
    int8_t piece;
    int8_t sq;
} PieceCapture;

typedef struct Move
{
    // from and to are mailbox indices
    int8_t from;
    int8_t to;
    PieceCapture capts[5];
    int8_t captsCount;
} Move;

void move_make(BoardState* state, Move* m);
void move_unmake(BoardState* state, Move* m);

int8_t move_gen(BoardState* state, Move* list);
int8_t move_genPiece(BoardState* state, Move* list, int8_t sq, int8_t val);
