static void newdesc_choose_black_squares(game_state *state,
                                         const int *shuffle_1toN)
{
    int const w = state->params.w, h = state->params.h, n = w * h;

    int k, any_white_cell, n_black_cells;

    for (k = 0; k < n; ++k) state->grid[k] = WHITE;

    any_white_cell = shuffle_1toN[n - 1];
    n_black_cells = 0;

    /* I like the puzzles that result from n / 3, but maybe this
     * could be made a (generation, i.e. non-full) parameter? */
    for (k = 0; k < n / 3; ++k) {
        int const i = shuffle_1toN[k], c = i % w, r = i / w;

        int j;
        for (j = 0; j < 4; ++j) {
            int const rr = r + dr[j], cc = c + dc[j], cell = idx(rr, cc, w);
            /* if you're out of bounds, we skip you */
            if (out_of_bounds(rr, cc, w, h)) continue;
            if (state->grid[cell] == BLACK) break; /* I can't be black */
        } if (j < 4) continue; /* I have black neighbour: I'm white */

        state->grid[i] = BLACK;
        ++n_black_cells;

        j = dfs_count_white(state, any_white_cell);
        if (j + n_black_cells < n) {
            state->grid[i] = WHITE;
            --n_black_cells;
        }
    }
}