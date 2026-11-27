#pragma once
#include "config.hpp"
#include "board.hpp"
#include "search.hpp"

constexpr int SEARCH_TIME_MS = 1000;

class Interface {
private:
    Board* board;
    Search search;
    bool running;
    bool auto_play;

public:
    Interface(Board* b) : board(b), search(b), running(false), auto_play(false) {}
    void help();
    void run();
    void run_command(char ch);
    void print_best_move();
    void play_best_move();
    void toggle_auto_play();
};
