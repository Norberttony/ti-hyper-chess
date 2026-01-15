#include "menu.h"
#include <graphx.h>
#include <sys/lcd.h>
#include "cursor.h"
#include "keypad-extras.h"

typedef struct Button
{
    MenuOption value;
    char* text;
} Button;

typedef struct Screen
{
    char* title;
    uint8_t buttonCount;
    Button buttons[];
} Screen;

// color indices
#define BUTTON_BG 93
#define TEXT_FG 4
#define BUTTON_SEL 119

// in pixels
#define BUTTON_W 100
#define BUTTON_H 20

Screen mainScreen =
{
    .title = "Hyper Chess",
    .buttonCount = 3,
    .buttons =
    {
        {
            .value = Menu_PlayLocal,
            .text = "Pass and Play"
        },
        {
            .value = Menu_PlayAI,
            .text = "Play vs AI"
        },
        {
            .value = Menu_Quit,
            .text = "Quit"
        }
    }
};

Screen colorScreen =
{
    .title = "Pick a color",
    .buttonCount = 4,
    .buttons =
    {
        {
            .value = Menu_PlayAIRdm,
            .text = "Random"
        },
        {
            .value = Menu_PlayAIWhite,
            .text = "White"
        },
        {
            .value = Menu_PlayAIBlack,
            .text = "Black"
        },
        {
            .value = Menu_GoBack,
            .text = "Go Back"
        }
    }
};

MenuOption menu_screen(Screen* screen)
{
    gfx_SetTextFGColor(TEXT_FG);
    uint8_t buttonCount = screen->buttonCount;
    Button* buttons = screen->buttons;
    char* title = screen->title;
    uint8_t select = 0;
    MenuOption opt = Menu_None;

    while (opt == Menu_None)
    {
        gfx_FillScreen(255);
    
        // draw title screen
        gfx_SetTextBGColor(255);
        gfx_SetTextScale(2, 2);
        gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth(title)) / 2, 10);
        gfx_PrintString(title);
        
        // draw buttons
        gfx_SetTextScale(1, 1);
        for (uint8_t i = 0; i < buttonCount; i++)
        {
            int y = 60 + 50 * i;
    
            uint8_t bg = select == i ? BUTTON_SEL : BUTTON_BG;
            Button* b = buttons + i;
            gfx_SetColor(bg);
            gfx_FillRectangle((LCD_WIDTH - BUTTON_W) / 2, y, BUTTON_W, BUTTON_H);
    
            gfx_SetTextBGColor(bg);
            gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth(b->text)) / 2, y + (BUTTON_H - 8) / 2);
            gfx_PrintString(b->text);
        }
    
        gfx_SwapDraw();
    
        key_update();
        if (key_wasJustReleased(kb_KeyEnter))
        {
            opt = buttons[select].value;
            break;
        }
        if (key_wasJustPressed(kb_KeyUp))
        {
            select--;
        }
        if (key_wasJustPressed(kb_KeyDown))
        {
            select++;
        }
        select %= buttonCount;
    }
    return opt;
}

MenuOption menu_loop(void)
{
    MenuOption opt = Menu_None;

    while (opt == Menu_None || opt == Menu_GoBack)
    {
        opt = menu_screen(&mainScreen);
        if (opt == Menu_PlayAI)
        {
            opt = menu_screen(&colorScreen);
        }
    }
    return opt;
}
