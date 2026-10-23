#pragma once
#include "config.hpp"
#include "board.hpp"
#include "search.hpp"

constexpr int SEARCH_DEPTH = 6;

class Interface {
private:
    Board* board;
    Search search;
    bool running;

public:
    Interface(Board* b) : board(b), search(b) {}
    void help();
    void run();
    void run_command(char ch);
    void print_best_move();
};
