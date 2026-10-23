#include "search.hpp"

SearchResult Search::find_best_move(int depth) {
    best_root_move = NULL_MOVE;
    int score = negamax(depth, 0);
    return {best_root_move, score};
}

int Search::negamax(int depth, int ply) {
    STATE state = board->get_state();
    if (state == DRAW) return DRAW_SCORE;
    if (state != RUNNING) return -(WIN_SCORE - ply);
    if (depth == 0) return evaluate();

    int best_score = -INFINITE_SCORE;
    Movepicker movepicker(board->generate_moves());
    Move move;

    while ((move = movepicker.next_move()) != NULL_MOVE) {
        board->make_move(move);
        int score = -negamax(depth - 1, ply + 1);
        board->undo_move();

        if (score > best_score) {
            best_score = score;
            if (ply == 0) best_root_move = move;
        }
    }

    return best_score;
}

int Search::evaluate() {
    return 0;
}
