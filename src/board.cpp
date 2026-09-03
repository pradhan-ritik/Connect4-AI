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
