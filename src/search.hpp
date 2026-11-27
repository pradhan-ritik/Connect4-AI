#pragma once
#include "config.hpp"
#include "board.hpp"
#include "movepicker.hpp"

constexpr int INFINITE_SCORE = 1000000;
constexpr int WIN_SCORE = 1000;
constexpr int DRAW_SCORE = 0;

constexpr int CENTER_WEIGHT = 3;
constexpr int TWO_WEIGHT = 2;
constexpr int THREE_WEIGHT = 5;
constexpr int OPPONENT_THREE_WEIGHT = 6;

struct WindowCounts {
    int twos;
    int threes;
};

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
    WindowCounts count_windows(BB own, BB opponent);

public:
    Search(Board* b) : board(b), best_root_move(NULL_MOVE) {}
    SearchResult find_best_move(int depth);
};
