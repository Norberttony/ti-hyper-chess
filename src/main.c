#include <graphx.h>
#include <sys/util.h>
#include <time.h>
#include "gfx/gfx.h"
#include "input.h"
#include "menu.h"
#include "defines.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetDrawBuffer();

    srandom(clock());

    while (1)
    {
        MenuOption opt = menu_loop();
        if (opt == Menu_PlayAIWhite)
        {
            input_boardLoop(1, black);
        }
        else if (opt == Menu_PlayAIBlack)
        {
            input_boardLoop(1, white);
        }
        else if (opt == Menu_PlayAIRdm)
        {
            // random :D
            uint8_t r = randInt(0, 1);
            if (r == 0)
            {
                input_boardLoop(1, white);
            }
            else
            {
                input_boardLoop(1, black);
            }
        }
        else if (opt == Menu_PlayLocal)
        {
            input_boardLoop(0, 0);
        }
        else if (opt == Menu_Quit)
        {
            break;
        }
    }

    gfx_End();

    return 0;
}
