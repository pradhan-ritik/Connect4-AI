# Connect4-AI

A Connect 4 engine written in C++ that you play in the terminal. Each player's pieces are stored as a bitboard (a 64-bit number with one bit per square). The engine finds its moves with a negamax search using alpha-beta pruning, move ordering, iterative deepening and a time limit, and scores positions it can't search further with a hand-written evaluation.

## Building

You only need `g++` and `make`. Every `src/*.cpp` file is compiled into a single `./ai` binary:

| Command | Flags | Use |
|---|---|---|
| `make` | `-Wall -Wextra` | normal development build |
| `make performance` | `-O3` | fastest build, for playing (asserts still run) |
| `make debug` | `-g -Wall -Wextra` | debugging with gdb |

```sh
make performance
./ai
```

## Playing

The game runs as a loop in the terminal. Type a command and press Enter:

| Command | Action |
|---|---|
| `a`–`g` | drop a piece in column A–G |
| `s` | show the engine's best move, its score and the depth it reached (does not play it) |
| `p` | let the engine play the best move |
| `m` | turn auto-play on or off (the engine replies to each of your moves) |
| `u` | undo the last move |
| `l` | print the bitboard of legal moves |
| `h` | show help |
| `q` | quit |

Each engine search gets 1 second (`SEARCH_TIME_MS` in `src/interface.hpp`). Column A is printed on the **right**:

```
🔴 turn
Gamestate: RUNNING
6 |    |    |    |    |    |    |    |
5 |    |    |    |    |    |    |    |
4 |    |    |    |    |    |    |    |
3 |    |    |    |    |    |    |    |
2 |    |    |    |    |    |    |    |
1 |    |    |    | 🔴 |    |    |    |
    G    F    E    D    C    B    A
```

Set `UNICODE_ALLOWED` in `src/config.hpp` to `false` to show pieces as `R`/`Y` instead of emoji.

You can also script commands by piping them in, e.g. `printf 'd\ns\nq\n' | ./ai`.

## How it works

### Bitboards (`config.hpp`, `bit_operations.hpp`)

The 6×7 board is stored in a `uint64_t` with **8 bits per row**: square index = `row * 8 + col`, with column A = bit 0 of each row. The 8th bit of each row is always empty. It stops horizontal and diagonal lines from wrapping around from one row to the next, so wins can be found with plain shifts and ANDs. Precomputed `ROW[]` and `COLUMN[]` masks, a compile-time `WINDOWS` table of all 69 lines of four squares, and bit helpers (`lsb`, `msb`, `pop_lsb`, …) live here. A `Move` is a column index 0–6.

### Board (`board.cpp/hpp`)

- `pieces[RED]` and `pieces[YELLOW]` bitboards, the side to move, and the game state (`RUNNING`, `DRAW`, `RED_WIN`, `YELLOW_WIN`).
- `make_move` drops a piece onto the top of a column. `update_state` then checks whether the player who just moved has won by shifting their bitboard along each direction (1, 8, 7 and 9 bits) and ANDing.
- `undo_move` takes the last column from the move history and removes that column's top piece.
- `generate_moves` returns the playable square in every column that isn't full, using one bit trick.
- `winning_squares(color)` returns every empty square that would complete four for `color`, including gaps such as `XX_X`.

### History (`history.cpp/hpp`)

A small move stack: each move takes 3 bits, packed into two `uint64_t`s. This is what lets `undo_move` work without saving copies of the board.

### Move ordering (`movepicker.cpp/hpp`)

`Movepicker` decides the order the search tries moves in:

1. the principal move (the best move from the previous depth), if it's legal
2. moves that win immediately
3. moves that block an opponent's win
4. quiet moves
5. bad moves (squares directly under an opponent's winning square)

Within each group, centre columns come first (D C E B F A G).

### Search (`search.cpp/hpp`)

- **Negamax with alpha-beta pruning**: scores are always from the side to move's point of view. A win is worth `WIN_SCORE - ply`, so faster wins score higher, and a draw is worth 0.
- **Iterative deepening**: the search runs at depth 1, 2, 3, … until it runs out of time, reaches the maximum depth or fills the board. Each depth searches the previous depth's best move first. Deepening stops early once a forced win or loss is found.
- **Time limit**: every 4096 nodes the search checks the clock. When time runs out, the unfinished depth is thrown away and the result from the last completed depth is used. Depth 1 always finishes.
- The search plays and undoes moves on the shared board, so the board is unchanged afterwards.

### Evaluation

When the search reaches its depth limit, `evaluate()` scores the position from the side to move's point of view, using the `WINDOWS` table:

- a bonus for pieces in the centre column (D)
- open twos and threes (lines of four holding 2 or 3 of a player's pieces and none of the opponent's), for both sides
- opponent threes weigh a bit more, so the engine prefers blocking

The weights (`CENTER_WEIGHT`, `TWO_WEIGHT`, `THREE_WEIGHT`, `OPPONENT_THREE_WEIGHT`) are in `search.hpp`. They stay far below `WIN_SCORE`, so the evaluation can never outweigh a forced win.

## Tests

There is no test framework. `src/tests.cpp` has hand-written functions (`test_BBs`, `test_board`, `test_history`, `test_movepicker`, `test_search`, `test_evaluation`, `test_iterative_deepening`) that print bitboards, boards and search results for you to check by eye. To run one, uncomment its call in `main()` in `src/main.cpp` (and comment out `interface.run()`), rebuild, and run `./ai`.

## Project layout

```
src/
  main.cpp              entry point
  config.hpp            types, constants, board masks, WINDOWS table
  bit_operations.hpp    bit helpers and bitboard printing
  board.cpp/hpp         board state, moves, win detection
  history.cpp/hpp       packed move stack
  movepicker.cpp/hpp    move ordering
  search.cpp/hpp        iterative deepening negamax and evaluation
  interface.cpp/hpp     terminal game loop
  tests.cpp/hpp         visual tests
```

## License

MIT. See [LICENSE](LICENSE).

## Future work: towards perfect play

### Why the engine isn't perfect yet

- With the 1 s budget (`SEARCH_TIME_MS` in `src/interface.hpp`) the search gets to about **depth 13–14** near the opening (`d`, `s` → depth 14; `d d d`, `s` → depth 13). A game is 42 plies long, so most decisions are made by `evaluate()` at the edge of what the search can see.
- **The main cause is depth.** Perfect play needs a search all the way to the end of the game. `negamax` has no transposition table, so it searches the same positions over and over (many move orders lead to the same position). Each new depth also starts over with a full window.
- **The evaluation makes it worse.** `evaluate()` counts open twos and threes, but it:
  - ignores **odd/even threat parity**: with best play, the first player wants threats on odd rows and the second player on even rows.
  - treats a three whose empty square can never be filled usefully the same as a real threat, and misses two threats stacked one above the other, which wins outright.
- Losing as second player doesn't prove the engine is weak. Connect 4 is a first-player win, so even a perfect engine loses to a first player who never makes a mistake.

### Is a full 42-ply search feasible?

Not by making the current search deeper: each extra ply costs roughly 3–5× more. But well-known Connect 4 solvers do search to the end of the game from the empty board in seconds to minutes. They use the techniques below. As the game goes on, fewer moves are left, so by around move 10–12 a full search to the end should fit in 1 s.

### Roadmap (most important first)

1. **Transposition table**: key from `pieces[RED]`/`pieces[YELLOW]` (or `position + mask`), storing score, depth, bound flag (exact/lower/upper) and best move. Pass the stored best move to `Movepicker` as `first_move` at every ply, not just the root.
2. **Null-window / MTD(f) search** to prove win, draw or loss without needing exact scores.
3. **Better move ordering**: try moves that create the most new `winning_squares` first, and prune moves that let the opponent win right away.
4. **Opening book** for the first ~8–12 plies, built offline with the solver, so the opening is instant and perfect.
5. **Parity-aware `evaluate()`** (odd/even threats, stacked threats, ignore dead threes) for positions that still can't be searched to the end.

### Verification

- Compare the depth printed by `s` at the opening and mid-game before and after each step.
- `test_search` and `test_iterative_deepening` should still give the same best moves and scores. Exact scores may change once search becomes win/draw/loss only; if they do, update the tests on purpose.
