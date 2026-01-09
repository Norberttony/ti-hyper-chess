#include "board-gfx.h"
#include <graphx.h>
#include "gfx/gfx.h"

gfx_sprite_t* pieceSprites[16] = {
    NULL,
    white_straddler,
    white_retractor,
    white_springer,
    white_coordinator,
    white_immobilizer,
    white_chameleon,
    white_king,
    NULL,
    black_straddler,
    black_retractor,
    black_springer,
    black_coordinator,
    black_immobilizer,
    black_chameleon,
    black_king
};

void drawBoardBG(BoardGFX* board)
{
    int sqSize = board->sqSize;
    int px = board->px;
    int py = board->py;
    int lightIdx = board->lightIdx;
    int darkIdx = board->darkIdx;

    int idx = lightIdx;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int sqX = x * sqSize + px;
            int sqY = y * sqSize + py;
            gfx_SetColor(idx);
            gfx_FillRectangle(sqX, sqY, sqSize, sqSize);
            idx = idx == lightIdx ? darkIdx : lightIdx;
        }
        idx = idx == lightIdx ? darkIdx : lightIdx;
    }
}

void drawPiece(BoardGFX* board, int x, int y, int piece)
{
    if (get_piece_type(piece) == 0)
    {
        return;
    }
    int sqSize = board->sqSize;
    int px = board->px + x * sqSize + (sqSize - 25) / 2;
    int py = board->py + y * sqSize + (sqSize - 25) - 1;
    gfx_TransparentSprite(pieceSprites[piece], px, py);
}

void drawBoardState(BoardGFX* board, BoardState* state)
{
    drawBoardBG(board);
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int idx = eightToTen(x, y);
            int piece = state->mailbox[idx];
            if (piece)
            {
                drawPiece(board, x, y, piece);
            }
        }
    }
}
