#pragma once
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <array>

typedef uint64_t BB;
typedef unsigned int uint;
typedef uint8_t Move;
// Allows cout to print Moves
inline std::ostream &operator<<(std::ostream &os, Move m) {
    return os << static_cast<unsigned int>(m);
}

enum STATE {
    RUNNING,
    DRAW,
    RED_WIN,
    YELLOW_WIN,
};

enum COLOR {
    RED,
    YELLOW,
};

constexpr Move NULL_MOVE = 64;
constexpr BB FULL = ~0ULL;
constexpr int nROWS = 6;
constexpr int nCOLUMNS = 7;
constexpr int STRIDE = 8;
constexpr int nSQUARES = nROWS * nCOLUMNS;
inline bool UNICODE_ALLOWED = true;



// bitboards
enum ROW { R1, R2, R3, R4, R5, R6 };
constexpr BB ROW_1 = 0b1111111ULL;
constexpr BB ROW_2 = ROW_1 << STRIDE;
constexpr BB ROW_3 = ROW_1 << STRIDE * 2;
constexpr BB ROW_4 = ROW_1 << STRIDE * 3;
constexpr BB ROW_5 = ROW_1 << STRIDE * 4;
constexpr BB ROW_6 = ROW_1 << STRIDE * 5;
constexpr BB ROW[6] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6};
constexpr BB ALL_SQUARES = ROW_1 | ROW_2 | ROW_3 | ROW_4 | ROW_5 | ROW_6;

enum COLUMN { A, B, C, D, E, F, G };
constexpr BB A_COL = 0b000000010000000100000001000000010000000100000001ULL;
constexpr BB B_COL = A_COL << 1;
constexpr BB C_COL = A_COL << 2;
constexpr BB D_COL = A_COL << 3;
constexpr BB E_COL = A_COL << 4;
constexpr BB F_COL = A_COL << 5;
constexpr BB G_COL = A_COL << 6;
constexpr BB COLUMN[7] = {A_COL, B_COL, C_COL, D_COL, E_COL, F_COL, G_COL};

// every line of 4 squares a player could win with
constexpr int nWINDOWS = 69;
constexpr std::array<BB, nWINDOWS> generate_windows() {
    std::array<BB, nWINDOWS> windows{};
    constexpr int row_steps[4] = {0, 1, 1, 1};
    constexpr int col_steps[4] = {1, 0, 1, -1};
    int count = 0;

    for (int direction = 0; direction < 4; ++direction) {
        for (int row = 0; row < nROWS; ++row) {
            for (int col = 0; col < nCOLUMNS; ++col) {
                int end_row = row + 3 * row_steps[direction];
                int end_col = col + 3 * col_steps[direction];
                if (end_row >= nROWS || end_col < 0 || end_col >= nCOLUMNS) continue;

                BB window = 0ULL;
                for (int i = 0; i < 4; ++i) {
                    int square = (row + i * row_steps[direction]) * STRIDE + col + i * col_steps[direction];
                    window |= 1ULL << square;
                }
                windows[count++] = window;
            }
        }
    }

    return windows;
}
constexpr std::array<BB, nWINDOWS> WINDOWS = generate_windows();
static_assert(WINDOWS[nWINDOWS - 1] != 0ULL);
