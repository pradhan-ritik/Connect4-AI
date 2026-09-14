#include "history.hpp"

void History::append(BB col) {
    assert(move_count <= 41);
    bool cb = current_bitboard();
    // appends move, if second bitboard -21 from "list" position
    // std::cout << 3*(move_count - 21*cb) << " " << cb << " ";
    hist[cb] |= col << offset(cb);
    move_count++;
}

Move History::get_last() {
    assert(move_count > 0);
    bool cb = current_bitboard(/*last_move=*/true);
    return (hist[cb] >> (offset(cb)-3)) & 0b111ULL;
}

Move History::pop_last() {
    assert(move_count > 0);
    Move last = get_last();
    bool cb = current_bitboard(/*last_move=*/true);
    hist[cb] &= FULL ^ (0b111ULL << (offset(cb)-3));
    move_count--;
    return last;
}
