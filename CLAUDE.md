# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & run

There is no build system beyond the `Makefile`; it compiles every `src/*.cpp` into a single `./ai` binary (git-ignored):

- `make` — warnings on (`-Wall -Wextra`)
- `make performance` — `-O3` (no warnings, asserts still active since `NDEBUG` is not defined)
- `make debug` — `-g` with warnings
- `./ai` — run

There is no test framework or linter. "Tests" are hand-written functions in `src/tests.cpp` (`test_BBs`, `test_board`, `test_history`, `test_movepicker`, `test_search`) that print bitboards/boards/search results for visual inspection. By default `main()` in `src/main.cpp` runs the interactive game (`Interface::run`) with the test calls commented out; to run a test, uncomment its call (and comment out the interface), rebuild, and run `./ai`. Interface commands can be scripted by piping input, e.g. `printf '4\n1\ns\nq\n' | ./ai`. New tests follow the same pattern: declare in `tests.hpp`, define in `tests.cpp`, call from `main()`.

## Architecture

A bitboard-based Connect 4 engine in C++ with a basic negamax search that is intended to be extended.

**Bitboard layout (`config.hpp`, `bit_operations.hpp`)** — the core convention everything depends on. The 6x7 board maps into a `uint64_t` (`BB`) with `STRIDE = 8` bits per row: square index = `row * 8 + col`, so bit 7 of each row is an unused padding bit that prevents horizontal/diagonal wins from wrapping across rows. Column A = col 0 = the lowest bit of each row. `print_BB`/`print_board` print column A on the **right** (labels read `G F E D C B A`). Precomputed `ROW[]`/`COLUMN[]` masks and bit helpers (`lsb`, `msb`, `pop_lsb`, `column()`, `row()`) live here. A `Move` (`uint8_t`) is a column index 0–6; `NULL_MOVE = 64` is the sentinel.

**Board (`board.cpp/hpp`)** — `pieces[RED]`, `pieces[YELLOW]` bitboards plus `turn`, `state` (`STATE` enum), and a `History`.
- `make_move(col)` drops a piece by finding the `msb` of the occupied column and placing one `STRIDE` above (or at row 0 if empty), then flips turn and calls `update_state()`.
- `update_state()` checks the player who *just moved* (`!turn`) using shift-and-AND win detection (shifts of 1, `STRIDE`, `STRIDE±1`).
- `undo_move()` pops the column from `History` and clears that column's top bit; it resets `state` to `RUNNING`.
- `generate_moves()` returns a bitboard of the playable square in each non-full column (`((full << STRIDE) ^ full ^ ROW_1) & ALL_SQUARES`; the `ALL_SQUARES` mask drops the bit a full column would leave above the board).

**History (`history.cpp/hpp`)** — a compact move stack: each move is 3 bits packed into two `uint64_t`s (moves 0–20 in `hist[0]`, 21–41 in `hist[1]`). Supports `append`, `get_last`, `pop_last`; this is what makes `undo_move` possible without storing full board copies.

**Movepicker (`movepicker.cpp/hpp`)** — iterator over a move bitboard from `generate_moves()`; `next_move()` yields column indices via `pop_lsb` until it returns `NULL_MOVE`. No move ordering yet — the intended hook for it.

**Search (`search.cpp/hpp`)** — `Search` holds a `Board*`; `find_best_move(depth)` runs plain negamax (no alpha-beta, ordering, or transposition table yet) and returns a `SearchResult {best_move, score}`. It plays and undoes moves on the shared board in place, so the board is unchanged afterwards. Scores are from the side to move: a terminal win for the previous mover returns `-(WIN_SCORE - ply)` (faster wins score higher), a draw `DRAW_SCORE`, and depth 0 calls `evaluate()`, which currently returns 0. The root move is recorded when `ply == 0`.

**Interface (`interface.cpp/hpp`)** — terminal loop holding a `Board*` and a `Search` on the same board: digits `1`–`7` play columns A–G, `s` prints the best move and score from a `SEARCH_DEPTH` search (defined in `interface.hpp`) without playing it, `u` undo, `m` print legal-move bitboard, `h` help, `q` quit.

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
