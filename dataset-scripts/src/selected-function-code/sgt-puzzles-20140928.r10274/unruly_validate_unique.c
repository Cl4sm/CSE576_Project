static int unruly_validate_unique(const game_state *state, int horizontal,
                                  int *errors)
{
    int w2 = state->w2, h2 = state->h2;

    int rmult = (horizontal ? w2 : 1);
    int cmult = (horizontal ? 1 : w2);
    int nr = (horizontal ? h2 : w2);
    int nc = (horizontal ? w2 : h2);
    int err = (horizontal ? FE_ROW_MATCH : FE_COL_MATCH);

    int r, r2, c;
    int ret = 0;

    /* Check for any two full rows matching exactly, and mark errors
     * accordingly (if required) */
    for (r = 0; r < nr; r++) {
        int nfull = 0;
        for (c = 0; c < nc; c++)
            if (state->grid[r*rmult + c*cmult] != EMPTY)
                nfull++;
        if (nfull != nc)
            continue;
        for (r2 = r+1; r2 < nr; r2++) {
            int match = TRUE;
            for (c = 0; c < nc; c++)
                if (state->grid[r*rmult + c*cmult] !=
                    state->grid[r2*rmult + c*cmult])
                    match = FALSE;
            if (match) {
                if (errors) {
                    for (c = 0; c < nc; c++) {
                        errors[r*rmult + c*cmult] |= err;
                        errors[r2*rmult + c*cmult] |= err;
                    }
                }
                ret++;
            }
        }
    }

    return ret;
}