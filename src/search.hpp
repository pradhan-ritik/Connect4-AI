#pragma once
#include "config.hpp"
#include "board.hpp"
#include "movepicker.hpp"

constexpr int INFINITE_SCORE = 1000000;
constexpr int WIN_SCORE = 1000;
constexpr int DRAW_SCORE = 0;

struct SearchResult {
    Move best_move;
    int score;
};

class Search {
private:
    Board* board;
    Move best_root_move;

    int negamax(int depth, int ply);
    int evaluate();

public:
    Search(Board* b) : board(b), best_root_move(NULL_MOVE) {}
    SearchResult find_best_move(int depth);
};
