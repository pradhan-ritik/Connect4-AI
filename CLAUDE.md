# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & run

There is no build system beyond the `Makefile`; it compiles every `src/*.cpp` into a single `./ai` binary (git-ignored):

- `make` — warnings on (`-Wall -Wextra`)
- `make performance` — `-O3` (no warnings, asserts still active since `NDEBUG` is not defined)
- `make debug` — `-g` with warnings
- `./ai` — run

There is no test framework or linter. "Tests" are hand-written functions in `src/tests.cpp` (`test_BBs`, `test_board`, `test_history`, `test_movepicker`, `test_search`, `test_evaluation`, `test_iterative_deepening`) that print bitboards/boards/search results for visual inspection. By default `main()` in `src/main.cpp` runs the interactive game (`Interface::run`) with the test calls commented out; to run a test, uncomment its call (and comment out the interface), rebuild, and run `./ai`. Interface commands can be scripted by piping input, e.g. `printf 'd\na\ns\nq\n' | ./ai`. New tests follow the same pattern: declare in `tests.hpp`, define in `tests.cpp`, call from `main()`.

## Architecture

A bitboard-based Connect 4 engine in C++ with a basic negamax search that is intended to be extended.

**Bitboard layout (`config.hpp`, `bit_operations.hpp`)** — the core convention everything depends on. The 6x7 board maps into a `uint64_t` (`BB`) with `STRIDE = 8` bits per row: square index = `row * 8 + col`, so bit 7 of each row is an unused padding bit that prevents horizontal/diagonal wins from wrapping across rows. Column A = col 0 = the lowest bit of each row. `print_BB`/`print_board` print column A on the **right** (labels read `G F E D C B A`). Precomputed `ROW[]`/`COLUMN[]` masks, the compile-time `WINDOWS` table (all 69 lines of 4 squares a player could win with), and bit helpers (`lsb`, `msb`, `pop_lsb`, `column()`, `row()`) live here. A `Move` (`uint8_t`) is a column index 0–6; `NULL_MOVE = 64` is the sentinel.

**Board (`board.cpp/hpp`)** — `pieces[RED]`, `pieces[YELLOW]` bitboards plus `turn`, `state` (`STATE` enum), and a `History`.
- `make_move(col)` drops a piece by finding the `msb` of the occupied column and placing one `STRIDE` above (or at row 0 if empty), then flips turn and calls `update_state()`.
- `update_state()` checks the player who *just moved* (`!turn`) using shift-and-AND win detection (shifts of 1, `STRIDE`, `STRIDE±1`).
- `undo_move()` pops the column from `History` and clears that column's top bit; it resets `state` to `RUNNING`.
- `get_turn()` and `get_pieces(color)` expose the side to move and each player's bitboard.
- `generate_moves()` returns a bitboard of the playable square in each non-full column (`((full << STRIDE) ^ full ^ ROW_1) & ALL_SQUARES`; the `ALL_SQUARES` mask drops the bit a full column would leave above the board).
- `winning_squares(color)` returns the empty squares that would complete four for `color` (shift-and-AND over the four directions, including gap patterns like `XX_X`); it ignores whether the square is playable yet.

**History (`history.cpp/hpp`)** — a compact move stack: each move is 3 bits packed into two `uint64_t`s (moves 0–20 in `hist[0]`, 21–41 in `hist[1]`). Supports `append`, `get_last`, `pop_last`; this is what makes `undo_move` possible without storing full board copies.

**Movepicker (`movepicker.cpp/hpp`)** — orders moves for the search. Constructed with `Movepicker(moves, own_threats, opponent_threats, first_move = NULL_MOVE)` (the threats come from `winning_squares`), it builds the order once: `first_move` if legal, then immediate wins, then blocks of opponent wins, then quiet moves, then bad moves (squares directly below an opponent winning square), each group center-first by `COLUMN_ORDER` (D C E B F A G). `next_move()` yields column indices until it returns `NULL_MOVE`.

**Search (`search.cpp/hpp`)** — `Search` holds a `Board*`; `find_best_move(max_depth, time_limit_ms = NO_TIME_LIMIT)` runs iterative deepening (depth 1, 2, … up to `max_depth` or the number of empty squares) over negamax with alpha-beta pruning and `Movepicker` move ordering (no transposition table yet), each depth from a full `-INFINITE_SCORE`/`INFINITE_SCORE` window, and returns a `SearchResult {best_move, score, depth}` from the deepest completed depth. The previous depth's best root move (`principal_move`) is searched first at the root. With a time limit, `negamax` checks the clock every `TIME_CHECK_INTERVAL` nodes and sets `stopped`, which unwinds the search and discards the unfinished depth; depth 1 always completes. Deepening stops early once a forced win or loss is found. Without a time limit (as in the tests) results are deterministic. It plays and undoes moves on the shared board in place, so the board is unchanged afterwards. Scores are from the side to move: a terminal win for the previous mover returns `-(WIN_SCORE - ply)` (faster wins score higher), a draw `DRAW_SCORE`, and depth 0 calls `evaluate()`. The root move is recorded when `ply == 0`; only strict improvements replace it, so ties keep the first move tried.

`evaluate()` scores from the side to move using `WINDOWS`: a center-column bonus (pieces in column D), twos and threes (windows with 2 or 3 of a player's pieces and none of the opponent's) for both sides, with opponent threes weighted slightly higher so the engine prefers blocking. The weights (`CENTER_WEIGHT`, `TWO_WEIGHT`, `THREE_WEIGHT`, `OPPONENT_THREE_WEIGHT`) are constants in `search.hpp` and must keep the total well below `WIN_SCORE - 42` so heuristics never outrank a forced win.

**Interface (`interface.cpp/hpp`)** — terminal loop holding a `Board*` and a `Search` on the same board: lower-case letters `a`–`g` play columns A–G, `s` prints the best move, score and depth reached from a `SEARCH_TIME_MS` search (defined in `interface.hpp`) without playing it, `p` plays that move, `m` toggles auto-play (the engine replies to each of your moves), `u` undo, `l` print legal-move bitboard, `h` help, `q` quit.

`UNICODE_ALLOWED` in `config.hpp` toggles emoji vs `R`/`Y` piece rendering.

## Coding conventions

- Keep comments to a minimum. Code should explain itself through clear function and variable names.
- The one exception: a block that has to be hard to read for efficiency (e.g. bit tricks) gets a single one-line comment saying what it does.

## Workflow

Work happens on issue-numbered branches (e.g. `20-movepicker`, `22-make-negamax`) merged into `main` via GitHub PRs.

Only commit when the user asks to. Before committing, run every test function in `src/tests.cpp` and check that they all still work: temporarily call all of them from `main()`, build with `make`, run `./ai`, check the printed output is correct, then restore `main()` to how it was.

Claude may edit this CLAUDE.md file as it sees fit, but only with the user's permission.

Commits and pull requests:
- Do not add a `Co-Authored-By: Claude` line, a "Generated with Claude Code" footer, or any other Claude/AI attribution to commits or PR descriptions; the user is the sole author.
- Commit each file separately (a matching `.cpp`/`.hpp` pair may share a commit), each with a brief message describing what changed in that file, e.g. `Add movepicker`, `Add tests for movepicker`.
