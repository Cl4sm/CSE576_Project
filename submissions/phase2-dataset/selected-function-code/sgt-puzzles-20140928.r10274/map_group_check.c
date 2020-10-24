static int map_group_check(game_state *state, int canon, int warn,
                           int *nislands_r)
{
    int *dsf = state->solver->dsf, nislands = 0;
    int x, y, i, allfull = 1;
    struct island *is;

    for (i = 0; i < state->n_islands; i++) {
        is = &state->islands[i];
        if (dsf_canonify(dsf, DINDEX(is->x,is->y)) != canon) continue;

        GRID(state, is->x, is->y) |= G_SWEEP;
        nislands++;
        if (island_countbridges(is) != is->count)
            allfull = 0;
    }
    if (warn && allfull && nislands != state->n_islands) {
        /* we're full and this island group isn't the whole set.
         * Mark all squares with this dsf canon as ERR. */
        for (x = 0; x < state->w; x++) {
            for (y = 0; y < state->h; y++) {
                if (dsf_canonify(dsf, DINDEX(x,y)) == canon) {
                    GRID(state,x,y) |= G_WARN;
                }
            }
        }

    }
    if (nislands_r) *nislands_r = nislands;
    return allfull;
}