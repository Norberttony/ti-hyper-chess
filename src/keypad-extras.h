#pragma once

#include <keypadc.h>

extern uint8_t* curr;
extern uint8_t* prev;

void key_update(void);

static inline uint16_t key_wasJustPressed(kb_lkey_t key)
{
    // is currently pressed and was not pressed before
    return curr[key >> 8] & key & ~(prev[key >> 8] & key);
}

static inline uint16_t key_wasJustReleased(kb_lkey_t key)
{
    // is not currently pressed and was pressed before
    return ~(curr[key >> 8] & key) & prev[key >> 8] & key;
}
