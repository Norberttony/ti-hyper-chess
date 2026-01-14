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
#include "move.h"
#include "defines.h"
#include "input.h"
#include "keypad-extras.h"
#include "engine.h"

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
        .isFlipped = 0
    };

    Cursor cursor = { 0 };
    cursor_init(&cursor, white_cursor);

    Move cache[MAX_MOVES];
    int8_t cacheSize = 0;

    Indicator from = { 0 };
    Indicator to = { 0 };

    int prev = clock();

    key_update();
    while (!kb_IsDown(kb_KeyClear))
    {
        if (state.toPlay == black)
        {
            gfx_FillScreen(255);
            boardgfx_drawState(&board, &state);
            gfx_SwapDraw();

            // engine plays!
            SearchResult r = thinkForDepth(&state, 3);
            move_make(&state, r.bestMove);
            prev = clock();
        }
        int curr = clock();
        float diff = (float)(curr - prev) / CLOCKS_PER_SEC;
        prev = curr;

        key_update();

        if (key_wasJustPressed(kb_Key2nd))
        {
            board.isFlipped = !board.isFlipped;
        }

        // perform state and graphical updates
        gfx_FillScreen(255);
        boardgfx_drawState(&board, &state);

        cacheSize = input_promptMoveStep(&cursor, &board, &state, &from, &to, cache, cacheSize);

        // move and draw cursor
        cursor_readInput(&cursor, 100.0f * diff);
        cursor_draw(&cursor);

        gfx_SwapDraw();
    }

    gfx_End();

    return 0;
}
