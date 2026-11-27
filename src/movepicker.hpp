#pragma once
#include "config.hpp"
#include "bit_operations.hpp"

constexpr Move COLUMN_ORDER[nCOLUMNS] = {D, C, E, B, F, A, G};

struct Movepicker {
private:
    Move ordered_moves[nCOLUMNS];
    int move_count;
    int next_index;

    void add_in_column_order(BB moves);
public:
    Movepicker(BB moves, BB own_threats, BB opponent_threats, Move first_move = NULL_MOVE);
    Move next_move();
};
