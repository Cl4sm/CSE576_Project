static int map_group_full(game_state *state, int *ngroups_r)
{
    int *dsf = state->solver->dsf, ngroups = 0;
    int i, anyfull = 0;
    struct island *is;

    /* NB this assumes map_group (or sth else) has cleared G_SWEEP. */

    for (i = 0; i < state->n_islands; i++) {
        is = &state->islands[i];
        if (GRID(state,is->x,is->y) & G_SWEEP) continue;

        ngroups++;
        if (map_group_check(state, dsf_canonify(dsf, DINDEX(is->x,is->y)),
                            1, NULL))
            anyfull = 1;
    }

    *ngroups_r = ngroups;
    return anyfull;
}