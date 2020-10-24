static int solve_island_subgroup(struct island *is, int direction)
{
    struct island *is_join;
    int nislands, *dsf = is->state->solver->dsf;
    game_state *state = is->state;

    debug(("..checking subgroups.\n"));

    /* if is isn't full, return 0. */
    if (island_countbridges(is) < is->count) {
        debug(("...orig island (%d,%d) not full.\n", is->x, is->y));
        return 0;
    }

    if (direction >= 0) {
        is_join = INDEX(state, gridi,
                        ISLAND_ORTHX(is, direction),
                        ISLAND_ORTHY(is, direction));
        assert(is_join);

        /* if is_join isn't full, return 0. */
        if (island_countbridges(is_join) < is_join->count) {
            debug(("...dest island (%d,%d) not full.\n",
                   is_join->x, is_join->y));
            return 0;
        }
    }

    /* Check group membership for is->dsf; if it's full return 1. */
    if (map_group_check(state, dsf_canonify(dsf, DINDEX(is->x,is->y)),
                        0, &nislands)) {
        if (nislands < state->n_islands) {
            /* we have a full subgroup that isn't the whole set.
             * This isn't allowed. */
            debug(("island at (%d,%d) makes full subgroup, disallowing.\n",
                   is->x, is->y));
            return 1;
        } else {
            debug(("...has finished puzzle.\n"));
        }
    }
    return 0;
}