static int solve_island_stage2(struct island *is, int *didsth_r)
{
    int added = 0, removed = 0, navail = 0, nadj, i;

    assert(didsth_r);

    for (i = 0; i < is->adj.npoints; i++) {
        if (solve_island_checkloop(is, i)) {
            debug(("removing possible loop at (%d,%d) direction %d.\n",
                   is->x, is->y, i));
            solve_join(is, i, -1, 0);
            map_update_possibles(is->state);
            removed = 1;
        } else {
            navail += island_isadj(is, i);
            /*debug(("stage2: navail for (%d,%d) direction (%d,%d) is %d.\n",
                   is->x, is->y,
                   is->adj.points[i].dx, is->adj.points[i].dy,
                   island_isadj(is, i)));*/
        }
    }

    /*debug(("island at (%d,%d) navail %d: checking...\n", is->x, is->y, navail));*/

    for (i = 0; i < is->adj.npoints; i++) {
        if (!island_hasbridge(is, i)) {
            nadj = island_isadj(is, i);
            if (nadj > 0 && (navail - nadj) < is->count) {
                /* we couldn't now complete the island without at
                 * least one bridge here; put it in. */
                /*debug(("nadj %d, navail %d, is->count %d.\n",
                       nadj, navail, is->count));*/
                debug(("island at (%d,%d) direction (%d,%d) must have 1 bridge\n",
                       is->x, is->y,
                       is->adj.points[i].dx, is->adj.points[i].dy));
                solve_join(is, i, 1, 0);
                added = 1;
                /*debug_state(is->state);
                debug_possibles(is->state);*/
            }
        }
    }
    if (added) map_update_possibles(is->state);
    if (added || removed) *didsth_r = 1;
    return 1;
}