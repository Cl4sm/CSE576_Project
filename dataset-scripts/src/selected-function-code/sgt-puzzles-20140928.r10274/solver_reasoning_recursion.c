static move *solver_reasoning_recursion(game_state *state,
                                        int nclues,
                                        const square *clues,
                                        move *buf)
{
    int const w = state->params.w, n = w * state->params.h;
    int cell, colour;

    for (cell = 0; cell < n; ++cell) {
        int const r = cell / w, c = cell % w;
        int i;
        game_state *newstate;
        move *recursive_result;

        if (state->grid[cell] != EMPTY) continue;

        /* FIXME: add enum alias for smallest and largest (or N) */
        for (colour = M_BLACK; colour <= M_WHITE; ++colour) {
            newstate = dup_game(state);
            newstate->grid[cell] = colour;
            recursive_result = do_solve(newstate, nclues, clues, buf,
                                        DIFF_RECURSION);
            free_game(newstate);
            if (recursive_result == NULL) {
                solver_makemove(r, c, M_BLACK + M_WHITE - colour, state, &buf);
                return buf;
            }
            for (i = 0; i < n && newstate->grid[i] != EMPTY; ++i);
            if (i == n) return buf;
        }
    }
    return buf;
}