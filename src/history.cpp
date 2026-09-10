#include "history.hpp"

void History::append(BB col) {
    assert(move_count <= 41);
    int cb = current_bitboard();
    // appends move, if second bitboard -21 from "list" position
    std::cout << 3*(move_count - 21*cb) << " " << cb << " ";
    hist[cb] |= col << 3*(move_count - 21*cb);
    move_count++;
}
