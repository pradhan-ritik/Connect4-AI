#include "board.hpp"

void Board::print_board() {
    BB full = full_board();
    bool cur;
    std::cout << "6 |";
    for (int i = nSQUARES-1; i > -1; i--) {
        cur = is_bit_active(full, i);
        if (cur == 0) {
            std::cout << "    |";
        }
        
        if (cur == 1) {
            if (is_bit_active(pieces[RED], i)) {
                std::cout << " ";
                print_red_square();
                std::cout << " |";
            }
            
            if (is_bit_active(pieces[YELLOW], i)) {
                std::cout << " ";
                print_yellow_square();
                std::cout << " |";
            }
        }

        if (i % nCOLUMNS == 0) {
            std::cout << "\n";
            if (i != 0) {
                std::cout << row(i-1) << " |";
            }
        }
    }

    std::cout << "    G    F    E    D    C    B    A\n";
}

void Board::make_move(Move col) {
    history.append(col);
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

void Board::undo_move() {
    next_turn();
    Move col = history.pop_last();
    uint highest = msb(full_board() & COLUMN[col]);
    set_bit_off(pieces[turn], highest);
}
