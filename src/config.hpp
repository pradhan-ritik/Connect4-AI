#pragma once
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cassert>

typedef uint64_t BB; // short for Bitboard
typedef unsigned int uint;

enum COLOR {
    RED,
    YELLOW,
};

constexpr int nROWS = 6;
constexpr int nCOLUMNS = 7;
constexpr int nSQUARES = nROWS * nCOLUMNS;


// bitboards
enum ROW { R1, R2, R3, R4, R5, R6 };
constexpr BB ROW_1 = 0b1111111ULL;
constexpr BB ROW_2 = ROW_1 << nCOLUMNS;
constexpr BB ROW_3 = ROW_1 << nCOLUMNS * 2;
constexpr BB ROW_4 = ROW_1 << nCOLUMNS * 3;
constexpr BB ROW_5 = ROW_1 << nCOLUMNS * 4;
constexpr BB ROW_6 = ROW_1 << nCOLUMNS * 5;
constexpr BB ROW[6] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6};

enum COLUMN { G, F, E, D, C, B, A };
constexpr BB A_COL = 0b100000010000001000000100000010000001ULL;
constexpr BB B_COL = A_COL << 1;
constexpr BB C_COL = A_COL << 2;
constexpr BB D_COL = A_COL << 3;
constexpr BB E_COL = A_COL << 4;
constexpr BB F_COL = A_COL << 5;
constexpr BB G_COL = A_COL << 6;
constexpr BB COLUMN[7] = {A_COL, B_COL, C_COL, D_COL, E_COL, F_COL, G_COL};
