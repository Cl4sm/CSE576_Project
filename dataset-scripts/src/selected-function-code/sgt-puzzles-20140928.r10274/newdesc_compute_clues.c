static void newdesc_compute_clues(game_state *state)
{
    int const w = state->params.w, h = state->params.h;
    int r, c;

    for (r = 0; r < h; ++r) {
        int run_size = 0, c, cc;
        for (c = 0; c <= w; ++c) {
            if (c == w || state->grid[idx(r, c, w)] == BLACK) {
                for (cc = c - run_size; cc < c; ++cc)
                    state->grid[idx(r, cc, w)] += run_size;
                run_size = 0;
            } else ++run_size;
        }
    }

    for (c = 0; c < w; ++c) {
        int run_size = 0, r, rr;
        for (r = 0; r <= h; ++r) {
            if (r == h || state->grid[idx(r, c, w)] == BLACK) {
                for (rr = r - run_size; rr < r; ++rr)
                    state->grid[idx(rr, c, w)] += run_size;
                run_size = 0;
            } else ++run_size;
        }
    }
}