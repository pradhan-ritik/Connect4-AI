#include "interface.hpp"

void Interface::help() {
    std::cout << "For playing: 1=A, 2=B, 3=C, 4=D, 5=E, 6=F, 7=G\n";
    std::cout << "u to undo\n";
    std::cout << "q to quit\n";
    // std::cout << "p to print board\n";
    std::cout << "h for this text\n";
}

void Interface::run() {
    running = true;
    help();
    while (running) {
        board->print_board();
        std::cout << ": ";
        char ch = getchar();
        while ('\n'!=getchar());
        std::cout << "\n";
        run_command(ch);
    }
}

void Interface::run_command(char ch) {
    // Move
    if (isdigit(ch)) {
        // convert '1' - '7' to 0 - 6
        Move m = ch - '1';
        if (!move_in_range(m)) goto invalid;
        if (board->get_state() != RUNNING) {
            std::cout << "The game is over! you can not make a move\n";
            return;
        }
        board->make_move(m);
    }

    else if (isalpha(ch)) {
        if (ch == 'q') running = false;
        else if (ch == 'h') help();
        // else if (ch == 'p') board->print_board();
        else if (ch == 'u') board->undo_move();
        else { goto invalid; }
    }


    // if everything works, then end, if something goes wrong, "goto failiure"
    return;
invalid:
    std::cout << "invalid command\n";
}
