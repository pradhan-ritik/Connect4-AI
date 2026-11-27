#pragma once
#include <chrono>
#include <algorithm>
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

constexpr int MAX_DEPTH = nSQUARES;
constexpr int NO_TIME_LIMIT = -1;
constexpr uint64_t TIME_CHECK_INTERVAL = 4096;

using Clock = std::chrono::steady_clock;

struct WindowCounts {
    int twos;
    int threes;
};

struct SearchResult {
    Move best_move;
    int score;
    int depth;
};

class Search {
private:
    Board* board;
    Move best_root_move;
    Move principal_move;
    uint64_t nodes;
    bool stopped;
    bool time_limited;
    Clock::time_point deadline;

    int negamax(int depth, int ply, int alpha, int beta);
    void check_time();
    int evaluate();
    WindowCounts count_windows(BB own, BB opponent);

public:
    Search(Board* b) : board(b), best_root_move(NULL_MOVE), principal_move(NULL_MOVE),
                       nodes(0), stopped(false), time_limited(false) {}
    SearchResult find_best_move(int max_depth, int time_limit_ms = NO_TIME_LIMIT);
};
