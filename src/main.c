#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include "board.h"
#include "gfx/gfx.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    drawBoardBG(4, 4, 29, 1, 2);

    while (!os_GetCSC());

    gfx_End();

    return 0;
}
