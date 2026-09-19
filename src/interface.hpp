#pragma once
#include "config.hpp"
#include "board.hpp"

class Interface {
private:
    Board* board;
    bool running;

public:
    Interface(Board* b) : board(b) {}
    void help();
    void run();
    void run_command(char ch);
};
