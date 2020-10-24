static int unruly_validate_counts(const game_state *state,
                                  struct unruly_scratch *scratch, int *errors)
{
    int w2 = state->w2, h2 = state->h2;
    int w = w2/2, h = h2/2;
    char below = FALSE;
    char above = FALSE;
    int i;

    /* See if all rows/columns are satisfied. If one is exceeded,
     * mark it as an error (if required)
     */

    char hasscratch = TRUE;
    if (!scratch) {
        scratch = unruly_new_scratch(state);
        hasscratch = FALSE;
    }

    for (i = 0; i < w2; i++) {
        if (scratch->ones_cols[i] < h)
            below = TRUE;
        if (scratch->zeros_cols[i] < h)
            below = TRUE;

        if (scratch->ones_cols[i] > h) {
            above = TRUE;
            if (errors)
                errors[2*h2 + i] = TRUE;
        } else if (errors)
            errors[2*h2 + i] = FALSE;

        if (scratch->zeros_cols[i] > h) {
            above = TRUE;
            if (errors)
                errors[2*h2 + w2 + i] = TRUE;
        } else if (errors)
            errors[2*h2 + w2 + i] = FALSE;
    }
    for (i = 0; i < h2; i++) {
        if (scratch->ones_rows[i] < w)
            below = TRUE;
        if (scratch->zeros_rows[i] < w)
            below = TRUE;

        if (scratch->ones_rows[i] > w) {
            above = TRUE;
            if (errors)
                errors[i] = TRUE;
        } else if (errors)
            errors[i] = FALSE;

        if (scratch->zeros_rows[i] > w) {
            above = TRUE;
            if (errors)
                errors[h2 + i] = TRUE;
        } else if (errors)
            errors[h2 + i] = FALSE;
    }

    if (!hasscratch)
        unruly_free_scratch(scratch);

    return (above ? -1 : below ? 1 : 0);
}