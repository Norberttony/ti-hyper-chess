#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include "board-gfx.h"
#include "board-state.h"
#include "gfx/gfx.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    BoardState state = { 0 };

    state_init(&state);

    BoardGFX board =
    {
        .px = 4,
        .py = 4,
        .sqSize = 29,
        .lightIdx = 1,
        .darkIdx = 2
    };

    drawBoardState(&board, &state);

    while (!os_GetCSC());

    gfx_End();

    return 0;
}
