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
};

// Win detection
inline bool _horizontal_east(BB bitboard) {
    return (bitboard) & (bitboard >> 1) & (bitboard >> 2) & (bitboard >> 3);
}

inline bool _horizontal_west(BB bitboard) {
    return (bitboard) & (bitboard << 1) & (bitboard << 2) & (bitboard << 3);
}

inline bool _vertical(BB bitboard) {
    return (bitboard) & (bitboard << (nCOLUMNS)) & (bitboard << (2*nCOLUMNS)) & (bitboard << (3*nCOLUMNS));
}

inline bool _diagonal_east(BB bitboard) {
    static const int NE = nCOLUMNS - 1;
    return (bitboard) & (bitboard << (NE)) & (bitboard << (2*NE)) & (bitboard << (3*NE));
}

inline bool _diagonal_west(BB bitboard) {
    static const int NW = nCOLUMNS + 1;
    return (bitboard) & (bitboard << (NW)) & (bitboard << (2*NW)) & (bitboard << (3*NW));
}
