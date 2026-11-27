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
    bool us = board->get_turn();
    BB own = board->get_pieces(us);
    BB opponent = board->get_pieces(!us);

    int center = count_bits(own & COLUMN[D]) - count_bits(opponent & COLUMN[D]);
    WindowCounts own_windows = count_windows(own, opponent);
    WindowCounts opponent_windows = count_windows(opponent, own);

    return CENTER_WEIGHT * center
         + TWO_WEIGHT * (own_windows.twos - opponent_windows.twos)
         + THREE_WEIGHT * own_windows.threes
         - OPPONENT_THREE_WEIGHT * opponent_windows.threes;
}

WindowCounts Search::count_windows(BB own, BB opponent) {
    WindowCounts counts = {0, 0};

    for (BB window : WINDOWS) {
        if (window & opponent) continue;

        uint own_in_window = count_bits(window & own);
        if (own_in_window == 2) counts.twos++;
        else if (own_in_window == 3) counts.threes++;
    }

    return counts;
}
