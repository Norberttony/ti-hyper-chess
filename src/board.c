#include "board.h"
#include <graphx.h>

void drawBoardBG(int px, int py, int sqSize, int darkIdx, int lightIdx)
{
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
