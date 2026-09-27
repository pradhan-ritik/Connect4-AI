#pragma once
#include "config.hpp"
#include "bit_operations.hpp"
#include "history.hpp"

inline void print_red_square() {
    if (UNICODE_ALLOWED)
        std::cout << "🔴";
    else
        std::cout << "R ";
}

inline void print_yellow_square() {
    if (UNICODE_ALLOWED)
        std::cout << "🟡";
    else
        std::cout << "Y ";
}

class Board {
private:
    History history;
    BB pieces[2];
    STATE state;
    bool turn;

    inline void next_turn() { turn = !turn; }
    inline BB full_board() { return pieces[RED] | pieces[YELLOW]; } 

public:
    Board() : history(History()), pieces{0ULL, 0ULL}, state(RUNNING), turn(RED) { }
    void print_board();
    void make_move(Move col);
    void undo_move();
    void update_state();

    inline STATE get_state() {
        return state;
    }
};

// Win detection
inline BB _horizontal(BB bitboard) {
    return bitboard
        & (bitboard << 1)
        & (bitboard << 2)
        & (bitboard << 3);
}

inline BB _vertical(BB bitboard) {
    return bitboard
        & (bitboard << STRIDE)
        & (bitboard << (2 * STRIDE))
        & (bitboard << (3 * STRIDE));
}

inline BB _diagonal_east(BB bitboard) {
    static constexpr int DIAGONAL_EAST = STRIDE - 1;

    return bitboard
        & (bitboard << DIAGONAL_EAST)
        & (bitboard << (2 * DIAGONAL_EAST))
        & (bitboard << (3 * DIAGONAL_EAST));
}

inline BB _diagonal_west(BB bitboard) {
    static constexpr int DIAGONAL_WEST = STRIDE + 1;

    return bitboard
        & (bitboard << DIAGONAL_WEST)
        & (bitboard << (2 * DIAGONAL_WEST))
        & (bitboard << (3 * DIAGONAL_WEST));
}
