static int unruly_fill_game(game_state *state, struct unruly_scratch *scratch,
                            random_state *rs)
{

    int w2 = state->w2, h2 = state->h2;
    int s = w2 * h2;
    int i, j;
    int *spaces;

#ifdef STANDALONE_SOLVER
    if (solver_verbose) {
        printf("Generator: Attempt to fill grid\n");
    }
#endif

    /* Generate random array of spaces */
    spaces = snewn(s, int);
    for (i = 0; i < s; i++)
        spaces[i] = i;
    shuffle(spaces, s, sizeof(*spaces), rs);

    /*
     * Construct a valid filled grid by repeatedly picking an unfilled
     * space and fill it, then calling the solver to fill in any
     * spaces forced by the change.
     */
    for (j = 0; j < s; j++) {
        i = spaces[j];

        if (state->grid[i] != EMPTY)
            continue;

        if (random_upto(rs, 2)) {
            state->grid[i] = N_ONE;
            scratch->ones_rows[i / w2]++;
            scratch->ones_cols[i % w2]++;
        } else {
            state->grid[i] = N_ZERO;
            scratch->zeros_rows[i / w2]++;
            scratch->zeros_cols[i % w2]++;
        }

        unruly_solve_game(state, scratch, DIFFCOUNT);
    }
    sfree(spaces);

    if (unruly_validate_all_rows(state, NULL) != 0
        || unruly_validate_counts(state, scratch, NULL) != 0)
        return FALSE;

    return TRUE;
}