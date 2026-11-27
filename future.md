# Future work: towards perfect play

## Why the engine isn't perfect yet

- With the 1 s budget (`SEARCH_TIME_MS` in `src/interface.hpp`) the search gets to about **depth 13–14** near the opening (`d`, `s` → depth 14; `d d d`, `s` → depth 13). A game is 42 plies long, so most decisions are made by `evaluate()` at the edge of what the search can see.
- **The main cause is depth.** Perfect play needs a search all the way to the end of the game. `negamax` has no transposition table, so it searches the same positions over and over (many move orders lead to the same position). Each new depth also starts over with a full window.
- **The evaluation makes it worse.** `evaluate()` counts open twos and threes, but it:
  - ignores **odd/even threat parity**: with best play, the first player wants threats on odd rows and the second player on even rows.
  - treats a three whose empty square can never be filled usefully the same as a real threat, and misses two threats stacked one above the other, which wins outright.
- Losing as second player doesn't prove the engine is weak. Connect 4 is a first-player win, so even a perfect engine loses to a first player who never makes a mistake.

## Is a full 42-ply search feasible?

Not by making the current search deeper: each extra ply costs roughly 3–5× more. But well-known Connect 4 solvers do search to the end of the game from the empty board in seconds to minutes. They use the techniques below. As the game goes on, fewer moves are left, so by around move 10–12 a full search to the end should fit in 1 s.

## Roadmap (most important first)

1. **Transposition table**: key from `pieces[RED]`/`pieces[YELLOW]` (or `position + mask`), storing score, depth, bound flag (exact/lower/upper) and best move. Pass the stored best move to `Movepicker` as `first_move` at every ply, not just the root.
2. **Null-window / MTD(f) search** to prove win, draw or loss without needing exact scores.
3. **Better move ordering**: try moves that create the most new `winning_squares` first, and prune moves that let the opponent win right away.
4. **Opening book** for the first ~8–12 plies, built offline with the solver, so the opening is instant and perfect.
5. **Parity-aware `evaluate()`** (odd/even threats, stacked threats, ignore dead threes) for positions that still can't be searched to the end.

## Verification

- Compare the depth printed by `s` at the opening and mid-game before and after each step.
- `test_search` and `test_iterative_deepening` should still give the same best moves and scores. Exact scores may change once search becomes win/draw/loss only; if they do, update the tests on purpose.
