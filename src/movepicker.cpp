#include "movepicker.hpp"

Movepicker::Movepicker(BB moves, BB own_threats, BB opponent_threats, Move first_move) : move_count(0), next_index(0) {
    if (first_move != NULL_MOVE && (moves & COLUMN[first_move])) {
        ordered_moves[move_count++] = first_move;
        moves &= ~COLUMN[first_move];
    }

    BB wins = moves & own_threats;
    BB blocks = moves & opponent_threats & ~wins;
    BB bad = moves & (opponent_threats >> STRIDE) & ~wins & ~blocks;
    BB quiet = moves & ~wins & ~blocks & ~bad;

    add_in_column_order(wins);
    add_in_column_order(blocks);
    add_in_column_order(quiet);
    add_in_column_order(bad);
}

void Movepicker::add_in_column_order(BB moves) {
    for (Move col : COLUMN_ORDER)
        if (moves & COLUMN[col]) ordered_moves[move_count++] = col;
}

Move Movepicker::next_move() {
    if (next_index == move_count) return NULL_MOVE;
    return ordered_moves[next_index++];
}
