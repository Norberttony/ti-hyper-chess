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

void boardgfx_drawBg(BoardGFX* board)
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

void boardgfx_drawPiece(BoardGFX* board, int x, int y, int piece)
{
    if (get_piece_type(piece) == 0)
    {
        return;
    }
    if (board->isFlipped)
    {
        x = 7 - x;
        y = 7 - y;
    }
    int sqSize = board->sqSize;
    int px = board->px + x * sqSize + (sqSize - 25) / 2;
    int py = board->py + y * sqSize + (sqSize - 25) - 1;
    gfx_TransparentSprite(pieceSprites[piece], px, py);
}

void boardgfx_drawState(BoardGFX* board, BoardState* state)
{
    boardgfx_drawBg(board);
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int idx = board_to_mailbox(x, y);
            int piece = state->mailbox[idx];
            if (piece)
            {
                boardgfx_drawPiece(board, x, y, piece);
            }
        }
    }
}

Square boardgfx_pxToGfxSq(BoardGFX* board, int x, int y)
{
    int ox = x - board->px;
    int oy = y - board->py;

    // since int division truncates, a -0.1 is 0 despite being on -1
    if (ox < 0)
    {
        ox -= board->sqSize;
    }
    if (oy < 0)
    {
        oy -= board->sqSize;
    }

    int sqX = ox / board->sqSize;
    int sqY = oy / board->sqSize;

    if (board->isFlipped)
    {
        sqX = 7 - sqX;
        sqY = 7 - sqY;
    }

    return (Square){
        .x = sqX,
        .y = sqY
    };
}

Square boardgfx_stateSqToGfxSq(int sq)
{
    // get mailbox (x, y)
    int mx = sq % MAILBOX_W;
    int my = sq / MAILBOX_W;

    // now convert mailbox coords to board coords
    int x = mx - MAILBOX_PADW;
    int y = my - MAILBOX_PADH;
    
    return (Square){
        .x = x,
        .y = y
    };
}

int boardgfx_isSqOutOfBounds(Square sq)
{
    return sq.x < 0 || sq.y < 0 || sq.x >= 8 || sq.y >= 8;
}

int boardgfx_areSquaresEqual(Square sq1, Square sq2)
{
    return sq1.x == sq2.x && sq1.y == sq2.y;
}
