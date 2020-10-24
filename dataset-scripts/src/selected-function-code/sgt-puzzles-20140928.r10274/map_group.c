static void map_group(game_state *state)
{
    int i, wh = state->w*state->h, d1, d2;
    int x, y, x2, y2;
    int *dsf = state->solver->dsf;
    struct island *is, *is_join;

    /* Initialise dsf. */
    dsf_init(dsf, wh);

    /* For each island, find connected islands right or down
     * and merge the dsf for the island squares as well as the
     * bridge squares. */
    for (x = 0; x < state->w; x++) {
        for (y = 0; y < state->h; y++) {
            GRID(state,x,y) &= ~(G_SWEEP|G_WARN); /* for group_full. */

            is = INDEX(state, gridi, x, y);
            if (!is) continue;
            d1 = DINDEX(x,y);
            for (i = 0; i < is->adj.npoints; i++) {
                /* only want right/down */
                if (is->adj.points[i].dx == -1 ||
                    is->adj.points[i].dy == -1) continue;

                is_join = island_find_connection(is, i);
                if (!is_join) continue;

                d2 = DINDEX(is_join->x, is_join->y);
                if (dsf_canonify(dsf,d1) == dsf_canonify(dsf,d2)) {
                    ; /* we have a loop. See comment in map_hasloops. */
                    /* However, we still want to merge all squares joining
                     * this side-that-makes-a-loop. */
                }
                /* merge all squares between island 1 and island 2. */
                for (x2 = x; x2 <= is_join->x; x2++) {
                    for (y2 = y; y2 <= is_join->y; y2++) {
                        d2 = DINDEX(x2,y2);
                        if (d1 != d2) dsf_merge(dsf,d1,d2);
                    }
                }
            }
        }
    }
}