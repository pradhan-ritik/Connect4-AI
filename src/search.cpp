#include "search.hpp"

SearchResult Search::find_best_move(int max_depth, int time_limit_ms) {
    time_limited = time_limit_ms != NO_TIME_LIMIT;
    deadline = Clock::now() + std::chrono::milliseconds(time_limit_ms);
    stopped = false;
    nodes = 0;
    principal_move = NULL_MOVE;

    int empty_squares = nSQUARES - count_bits(board->get_pieces(RED) | board->get_pieces(YELLOW));
    SearchResult result = {NULL_MOVE, 0, 0};

    for (int depth = 1; depth <= std::min(max_depth, empty_squares); ++depth) {
        best_root_move = NULL_MOVE;
        int score = negamax(depth, 0, -INFINITE_SCORE, INFINITE_SCORE);
        if (stopped) break;

        result = {best_root_move, score, depth};
        principal_move = best_root_move;
        if (std::abs(score) >= WIN_SCORE - nSQUARES) break;
    }

    return result;
}

void Search::check_time() {
    if (time_limited && principal_move != NULL_MOVE && Clock::now() >= deadline) stopped = true;
}

int Search::negamax(int depth, int ply, int alpha, int beta) {
    if (++nodes % TIME_CHECK_INTERVAL == 0) check_time();
    if (stopped) return 0;

    STATE state = board->get_state();
    if (state == DRAW) return DRAW_SCORE;
    if (state != RUNNING) return -(WIN_SCORE - ply);
    if (depth == 0) return evaluate();

    int best_score = -INFINITE_SCORE;
    bool us = board->get_turn();
    Move first_move = ply == 0 ? principal_move : NULL_MOVE;
    Movepicker movepicker(board->generate_moves(), board->winning_squares(us), board->winning_squares(!us), first_move);
    Move move;

    while ((move = movepicker.next_move()) != NULL_MOVE) {
        board->make_move(move);
        int score = -negamax(depth - 1, ply + 1, -beta, -alpha);
        board->undo_move();
        if (stopped) return 0;

        if (score > best_score) {
            best_score = score;
            if (ply == 0) best_root_move = move;
        }

        if (score > alpha) alpha = score;
        if (alpha >= beta) break;
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
