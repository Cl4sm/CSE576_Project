static int unruly_solver_check_uniques(game_state *state, int *rowcount,
                                       int horizontal, char check, char block,
                                       struct unruly_scratch *scratch)
{
    int w2 = state->w2, h2 = state->h2;

    int rmult = (horizontal ? w2 : 1);
    int cmult = (horizontal ? 1 : w2);
    int nr = (horizontal ? h2 : w2);
    int nc = (horizontal ? w2 : h2);
    int max = nc / 2;

    int r, r2, c;
    int ret = 0;

    /*
     * Find each row that has max entries of type 'check', and see if
     * all those entries match those in any row with max-1 entries. If
     * so, set the last non-matching entry of the latter row to ensure
     * that it's different.
     */
    for (r = 0; r < nr; r++) {
        if (rowcount[r] != max)
            continue;
        for (r2 = 0; r2 < nr; r2++) {
            int nmatch = 0, nonmatch = -1;
            if (rowcount[r2] != max-1)
                continue;
            for (c = 0; c < nc; c++) {
                if (state->grid[r*rmult + c*cmult] == check) {
                    if (state->grid[r2*rmult + c*cmult] == check)
                        nmatch++;
                    else
                        nonmatch = c;
                }
            }
            if (nmatch == max-1) {
                int i1 = r2 * rmult + nonmatch * cmult;
                assert(nonmatch != -1);
                if (state->grid[i1] == block)
                    continue;
                assert(state->grid[i1] == EMPTY);
#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: matching %s %i, %i gives %c at %i,%i\n",
                           horizontal ? "rows" : "cols",
                           r, r2, (block == N_ONE ? '1' : '0'), i1 % w2,
                           i1 / w2);
                }
#endif
                state->grid[i1] = block;
                if (block == N_ONE) {
                    scratch->ones_rows[i1 / w2]++;
                    scratch->ones_cols[i1 % w2]++;
                } else {
                    scratch->zeros_rows[i1 / w2]++;
                    scratch->zeros_cols[i1 % w2]++;
                }
                ret++;
            }
        }
    }
    return ret;
}