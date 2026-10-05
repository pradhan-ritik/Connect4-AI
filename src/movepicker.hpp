#pragma once
#include "config.hpp"
#include "bit_operations.hpp"

struct Movepicker {
private:
    BB moves;
    Move extract_move(BB mv) { return Move(column(lsb(mv))); }
public:
    Movepicker(BB move_bb) : moves(move_bb) { }
    Move next_move();
};
