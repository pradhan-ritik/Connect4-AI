#pragma once
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>

typedef uint64_t BB; // short for Bitboard
typedef unsigned int uint;

enum COLOR {
    RED,
    YELLOW,
};

constexpr uint nROWS = 6;
constexpr uint nCOLUMNS = 7;
constexpr uint nSQUARES = nROWS * nCOLUMNS;


// bitboards
constexpr BB ROW_1 = 0b1111111ULL;
constexpr BB ROW_2 = ROW_1 << nCOLUMNS;
constexpr BB ROW_3 = ROW_1 << nCOLUMNS * 2;
constexpr BB ROW_4 = ROW_1 << nCOLUMNS * 3;
constexpr BB ROW_5 = ROW_1 << nCOLUMNS * 4;
constexpr BB ROW_6 = ROW_1 << nCOLUMNS * 5;

constexpr BB A_COL = 0b100000010000001000000100000010000001000000;
constexpr BB B_COL = A_COL >> 1;
constexpr BB C_COL = A_COL >> 2;
constexpr BB D_COL = A_COL >> 3;
constexpr BB E_COL = A_COL >> 4;
constexpr BB F_COL = A_COL >> 5;
constexpr BB G_COL = A_COL >> 6;
