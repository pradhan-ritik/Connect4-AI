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
    BB generate_moves();
    BB winning_squares(bool color);
    void update_state();

    inline STATE get_state() {
        return state;
    }

    inline bool get_turn() {
        return turn;
    }

    inline BB get_pieces(bool color) {
        return pieces[color];
    }
};
