#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include "board.h"
#include "gfx/gfx.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    BoardGFX board =
    {
        .px = 4,
        .py = 4,
        .sqSize = 29
    };

    drawBoardBG(&board, 1, 2);

    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            drawPiece(&board, x, y, y * 8 + x);
        }
    }

    while (!os_GetCSC());

    gfx_End();

    return 0;
}
