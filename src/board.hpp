#pragma once
#include "config.hpp"
#include "bit_operations.hpp"

inline void print_red_square() {
    if (UNICODE_ALLOWED)
        std::cout << "🔴";
    else
        std::cout << "R ";
}

inline void print_yellow_square() {
    if (UNICODE_ALLOWED)
        std::cout << "🟡";
    else
        std::cout << "Y ";
}

class Board {
private:
    BB pieces[2];
    bool turn;

    inline void next_turn() { turn = !turn; }
    inline BB full_board() { return pieces[RED] | pieces[YELLOW]; } 

public:
    Board() : pieces{0ULL, 0ULL}, turn(RED) { }
    void print_board();
    void make_move(uint pos);
};
