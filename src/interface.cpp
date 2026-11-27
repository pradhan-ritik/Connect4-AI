#include "interface.hpp"

void Interface::help() {
    std::cout << "For playing: type the column letter, a-g\n";
    std::cout << "s to show the best move\n";
    std::cout << "p to play the best move\n";
    std::cout << "m to switch mode (engine automatically replies to your moves)\n";
    std::cout << "l to show the legal moves\n";
    std::cout << "u to undo\n";
    std::cout << "q to quit\n";
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
    if (ch >= 'a' && ch <= 'g') {
        if (board->get_state() != RUNNING) {
            std::cout << "The game is over! you can not make a move\n";
            return;
        }
        board->make_move(ch - 'a');
        if (auto_play && board->get_state() == RUNNING) play_best_move();
    }

    else if (isalpha(ch)) {
        if (ch == 'q') running = false;
        else if (ch == 'h') help();
        else if (ch == 's') print_best_move();
        else if (ch == 'p') play_best_move();
        else if (ch == 'u') board->undo_move();
        else if (ch == 'm') toggle_auto_play();
        else if (ch == 'l') print_BB(board->generate_moves());
        else { goto invalid; }
    }
    else { goto invalid; }


    // if everything works, then end, if something goes wrong, "goto failiure"
    return;
invalid:
    std::cout << "invalid command\n";
}

void Interface::print_best_move() {
    if (board->get_state() != RUNNING) {
        std::cout << "The game is over! there is no best move\n";
        return;
    }

    SearchResult result = search.find_best_move(MAX_DEPTH, SEARCH_TIME_MS);
    std::cout << "Best move: " << char('A' + result.best_move) << " (score " << result.score << ", depth " << result.depth << ")\n";
}

void Interface::play_best_move() {
    if (board->get_state() != RUNNING) {
        std::cout << "The game is over! you can not make a move\n";
        return;
    }

    SearchResult result = search.find_best_move(MAX_DEPTH, SEARCH_TIME_MS);
    std::cout << "Engine plays: " << char('A' + result.best_move) << " (score " << result.score << ", depth " << result.depth << ")\n";
    board->make_move(result.best_move);
}

void Interface::toggle_auto_play() {
    auto_play = !auto_play;
    if (auto_play) std::cout << "Mode: engine automatically replies to your moves\n";
    else std::cout << "Mode: manual\n";
}
