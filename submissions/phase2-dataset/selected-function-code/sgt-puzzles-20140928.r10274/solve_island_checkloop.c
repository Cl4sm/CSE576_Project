static int solve_island_checkloop(struct island *is, int direction)
{
    struct island *is_orth;
    int *dsf = is->state->solver->dsf, d1, d2;
    game_state *state = is->state;

    if (is->state->allowloops) return 0; /* don't care anyway */
    if (island_hasbridge(is, direction)) return 0; /* already has a bridge */
    if (island_isadj(is, direction) == 0) return 0; /* no adj island */

    is_orth = INDEX(is->state, gridi,
                    ISLAND_ORTHX(is,direction),
                    ISLAND_ORTHY(is,direction));
    if (!is_orth) return 0;

    d1 = DINDEX(is->x, is->y);
    d2 = DINDEX(is_orth->x, is_orth->y);
    if (dsf_canonify(dsf, d1) == dsf_canonify(dsf, d2)) {
        /* two islands are connected already; don't join them. */
        return 1;
    }
    return 0;
}