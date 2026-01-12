#include "keypad-extras.h"

uint8_t buff1[8] = { 0 };
uint8_t buff2[8] = { 0 };

uint8_t* curr = buff1;
uint8_t* prev = buff2;

void key_update(void)
{
    kb_Scan();

    // swap buffers
    uint8_t* temp = curr;
    curr = prev;
    prev = temp;

    // copy new values over
    for (uint8_t i = 1; i < 8; i++)
    {
        curr[i] = kb_Data[i];
    }
}
