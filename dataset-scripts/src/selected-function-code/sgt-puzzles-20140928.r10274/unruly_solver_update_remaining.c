static void unruly_solver_update_remaining(const game_state *state,
                                           struct unruly_scratch *scratch)
{
    int w2 = state->w2, h2 = state->h2;
    int x, y;

    /* Reset all scratch data */
    memset(scratch->ones_rows, 0, h2 * sizeof(int));
    memset(scratch->ones_cols, 0, w2 * sizeof(int));
    memset(scratch->zeros_rows, 0, h2 * sizeof(int));
    memset(scratch->zeros_cols, 0, w2 * sizeof(int));

    for (x = 0; x < w2; x++)
        for (y = 0; y < h2; y++) {
            if (state->grid[y * w2 + x] == N_ONE) {
                scratch->ones_rows[y]++;
                scratch->ones_cols[x]++;
            } else if (state->grid[y * w2 + x] == N_ZERO) {
                scratch->zeros_rows[y]++;
                scratch->zeros_cols[x]++;
            }
        }
}