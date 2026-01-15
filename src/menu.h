#pragma once

typedef enum MenuOption
{
    Menu_None,
    Menu_PlayLocal,
    Menu_PlayAI,
    Menu_PlayAIRdm,
    Menu_PlayAIWhite,
    Menu_PlayAIBlack,
    Menu_Quit,
    Menu_GoBack
} MenuOption;

MenuOption menu_loop(void);
