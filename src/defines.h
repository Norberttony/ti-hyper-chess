#pragma once

#include <stdint.h>

// sides
enum
{
    white = 0,
    black = 8
};

// piece types
enum
{
    _,
    straddler,
    retractor,
    springer,
    coordinator,
    immobilizer,
    chameleon,
    king
};

// easier representation of square indices for mailbox
enum
{
    a8 = 11, b8, c8, d8, e8, f8, g8, h8,
    a7 = 21, b7, c7, d7, e7, f7, g7, h7,
    a6 = 31, b6, c6, d6, e6, f6, g6, h6,
    a5 = 41, b5, c5, d5, e5, f5, g5, h5,
    a4 = 51, b4, c4, d4, e4, f4, g4, h4,
    a3 = 61, b3, c3, d3, e3, f3, g3, h3,
    a2 = 71, b2, c2, d2, e2, f2, g2, h2,
    a1 = 81, b1, c1, d1, e1, f1, g1, h1
};

static inline int8_t get_opposing_side(int8_t toPlay)
{
    return !toPlay << 3;
}
