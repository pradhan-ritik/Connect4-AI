#include "board.hpp"

// Win detection
inline BB horizontal(BB bitboard) {
    return bitboard
        & (bitboard << 1)
        & (bitboard << 2)
        & (bitboard << 3);
}

inline BB vertical(BB bitboard) {
    return bitboard
        & (bitboard << STRIDE)
        & (bitboard << (2 * STRIDE))
        & (bitboard << (3 * STRIDE));
}

inline BB diagonal_east(BB bitboard) {
    static constexpr int DIAGONAL_EAST = STRIDE - 1;

    return bitboard
        & (bitboard << DIAGONAL_EAST)
        & (bitboard << (2 * DIAGONAL_EAST))
        & (bitboard << (3 * DIAGONAL_EAST));
}

inline BB diagonal_west(BB bitboard) {
    static constexpr int DIAGONAL_WEST = STRIDE + 1;

    return bitboard
        & (bitboard << DIAGONAL_WEST)
        & (bitboard << (2 * DIAGONAL_WEST))
        & (bitboard << (3 * DIAGONAL_WEST));
}

void Board::print_board() {
    BB full = full_board();

    if (turn == RED) {
        print_red_square();
        std::cout << " turn\n";
    }
    else {
        print_yellow_square();
        std::cout << " turn\n";
    }

    std::cout << "Gamestate: ";

    switch (state) {
        case RUNNING:
            std::cout << "RUNNING\n";
            break;

        case DRAW:
            std::cout << "DRAW\n";
            break;

        case RED_WIN:
            print_red_square();
            std::cout << "WIN\n";
            break;

        case YELLOW_WIN:
            print_yellow_square();
            std::cout << "WIN\n";
            break;
    }

    for (int row = nROWS - 1; row >= 0; --row) {
        std::cout << row + 1 << " |";

        for (int col = nCOLUMNS - 1; col >= 0; --col) {
            uint pos = row * STRIDE + col;

            if (!is_bit_active(full, pos)) {
                std::cout << "    |";
            }
            else if (is_bit_active(pieces[RED], pos)) {
                std::cout << " ";
                print_red_square();
                std::cout << " |";
            }
            else {
                std::cout << " ";
                print_yellow_square();
                std::cout << " |";
            }
        }

        std::cout << "\n";
    }

    std::cout << "    G    F    E    D    C    B    A\n";
}

void Board::make_move(Move col) {
    assert(move_in_range(col));

    history.append(col);
    BB column = full_board() & COLUMN[col];
    uint highest = msb(column);

    if (!column) {
        set_bit_on(pieces[turn], col);
        goto postmove;
    }

    highest += STRIDE;

    assert(highest < nROWS * STRIDE);
    set_bit_on(pieces[turn], highest);
postmove:
    next_turn();
    update_state();
}

void Board::undo_move() {
    next_turn();
    Move col = history.pop_last();
    uint highest = msb(full_board() & COLUMN[col]);
    set_bit_off(pieces[turn], highest);
    state = RUNNING;
}

void Board::update_state() {
    // do not return incase a color has won even though all the squares are taken up
    if (history.get_move_count() == nSQUARES) state = DRAW;
    // supposed to run after make_move(), so it will analyse the state for the previous color
    bool color = !turn;
    BB bitboard = pieces[color];
    bool win =  horizontal(bitboard) |
                vertical(bitboard) |
                diagonal_east(bitboard) |
                diagonal_west(bitboard);

    if (win && color == RED) state = RED_WIN;
    if (win && color == YELLOW) state = YELLOW_WIN;
}
