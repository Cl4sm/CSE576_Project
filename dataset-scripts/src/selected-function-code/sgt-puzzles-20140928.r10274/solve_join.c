static void solve_join(struct island *is, int direction, int n, int is_max)
{
    struct island *is_orth;
    int d1, d2, *dsf = is->state->solver->dsf;
    game_state *state = is->state; /* for DINDEX */

    is_orth = INDEX(is->state, gridi,
                    ISLAND_ORTHX(is, direction),
                    ISLAND_ORTHY(is, direction));
    assert(is_orth);
    /*debug(("...joining (%d,%d) to (%d,%d) with %d bridge(s).\n",
           is->x, is->y, is_orth->x, is_orth->y, n));*/
    island_join(is, is_orth, n, is_max);

    if (n > 0 && !is_max) {
        d1 = DINDEX(is->x, is->y);
        d2 = DINDEX(is_orth->x, is_orth->y);
        if (dsf_canonify(dsf, d1) != dsf_canonify(dsf, d2))
            dsf_merge(dsf, d1, d2);
    }
}