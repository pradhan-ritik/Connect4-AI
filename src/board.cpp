#include "board.hpp"

void Board::print_board() {
    BB full = full_board();
    bool cur;
    for (int i = nSQUARES-1; i > -1; i--) {
        cur = is_bit_active(full, i);
        if (cur == 0) {
            std::cout << "0 ";
        }
        
        if (cur == 1) {
            if (is_bit_active(pieces[RED], i)) {
                std::cout << "R ";
            }
            
            if (is_bit_active(pieces[YELLOW], i)) {
                std::cout << "Y ";
            }
        }

        if (i % nCOLUMNS == 0) {
            std::cout << "\n";
        }
    }
}

void Board::make_move(uint col) {
    uint highest = msb(full_board() & COLUMN[col]);
    if (highest == 0) {
        set_bit_on(pieces[turn], col);   
        next_turn();
        return;
    }
    highest += nCOLUMNS;

    assert(highest < nSQUARES);
    set_bit_on(pieces[turn], highest);
    next_turn();
}
