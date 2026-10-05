#include "movepicker.hpp"

Move Movepicker::next_move() {
    if (!moves) return NULL_MOVE;
    // No ordering for now
    Move mv = extract_move(moves);
    pop_lsb(moves);
    return mv;
}
