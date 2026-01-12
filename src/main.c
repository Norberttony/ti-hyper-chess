#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <keypadc.h>
#include <time.h>
#include "board-gfx.h"
#include "board-state.h"
#include "cursor.h"
#include "indicator.h"
#include "gfx/gfx.h"
#include "move-gen.h"
#include "defines.h"
#include "input.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetDrawBuffer();

    BoardState state = { 0 };
    state_init(&state);

    BoardGFX board =
    {
        .px = 4,
        .py = 4,
        .sqSize = 29,
        .lightIdx = 2,
        .darkIdx = 1,
        .isFlipped = 1
    };

    Cursor cursor = { 0 };
    cursor_init(&cursor, white_cursor);

    Indicator from = { 0 };
    Indicator to = { 0 };

    int prev = clock();

    kb_Scan();
    while (!kb_IsDown(kb_KeyClear))
    {
        int curr = clock();
        float diff = (float)(curr - prev) / CLOCKS_PER_SEC;
        prev = curr;

        if (kb_IsDown(kb_Key2nd))
        {
            board.isFlipped = !board.isFlipped;
        }

        // perform state and graphical updates
        gfx_FillScreen(255);
        boardgfx_drawState(&board, &state);

        input_promptMoveStep(&cursor, &board, &state, &from, &to);

        // move and draw cursor
        cursor_readInput(&cursor, 100.0f * diff);
        cursor_draw(&cursor);

        gfx_SwapDraw();
        kb_Scan();
    }

    gfx_End();

    return 0;
}
