static int solve_island_stage3(struct island *is, int *didsth_r)
{
    int i, n, x, y, missing, spc, curr, maxb, didsth = 0;
    int wh = is->state->w * is->state->h;
    struct solver_state *ss = is->state->solver;

    assert(didsth_r);

    missing = is->count - island_countbridges(is);
    if (missing <= 0) return 1;

    for (i = 0; i < is->adj.npoints; i++) {
        x = is->adj.points[i].x;
        y = is->adj.points[i].y;
        spc = island_adjspace(is, 1, missing, i);
        if (spc == 0) continue;

        curr = GRIDCOUNT(is->state, x, y,
                         is->adj.points[i].dx ? G_LINEH : G_LINEV);
        debug(("island at (%d,%d) s3, trying %d - %d bridges.\n",
               is->x, is->y, curr+1, curr+spc));

        /* Now we know that this island could have more bridges,
         * to bring the total from curr+1 to curr+spc. */
        maxb = -1;
        /* We have to squirrel the dsf away and restore it afterwards;
         * it is additive only, and can't be removed from. */
        memcpy(ss->tmpdsf, ss->dsf, wh*sizeof(int));
        for (n = curr+1; n <= curr+spc; n++) {
            solve_join(is, i, n, 0);
            map_update_possibles(is->state);

            if (solve_island_subgroup(is, i) ||
                solve_island_impossible(is->state)) {
                maxb = n-1;
                debug(("island at (%d,%d) d(%d,%d) new max of %d bridges:\n",
                       is->x, is->y,
                       is->adj.points[i].dx, is->adj.points[i].dy,
                       maxb));
                break;
            }
        }
        solve_join(is, i, curr, 0); /* put back to before. */
        memcpy(ss->dsf, ss->tmpdsf, wh*sizeof(int));

        if (maxb != -1) {
            /*debug_state(is->state);*/
            if (maxb == 0) {
                debug(("...adding NOLINE.\n"));
                solve_join(is, i, -1, 0); /* we can't have any bridges here. */
            } else {
                debug(("...setting maximum\n"));
                solve_join(is, i, maxb, 1);
            }
            didsth = 1;
        }
        map_update_possibles(is->state);
    }

    for (i = 0; i < is->adj.npoints; i++) {
        /*
         * Now check to see if any currently empty direction must have
         * at least one bridge in order to avoid forming an isolated
         * subgraph. This differs from the check above in that it
         * considers multiple target islands. For example:
         *
         *   2   2    4
         *                                  1     3     2
         *       3
         *                                        4
         *
         * The example on the left can be handled by the above loop:
         * it will observe that connecting the central 2 twice to the
         * left would form an isolated subgraph, and hence it will
         * restrict that 2 to at most one bridge in that direction.
         * But the example on the right won't be handled by that loop,
         * because the deduction requires us to imagine connecting the
         * 3 to _both_ the 1 and 2 at once to form an isolated
         * subgraph.
         *
         * This pass is necessary _as well_ as the above one, because
         * neither can do the other's job. In the left one,
         * restricting the direction which _would_ cause trouble can
         * be done even if it's not yet clear which of the remaining
         * directions has to have a compensatory bridge; whereas the
         * pass below that can handle the right-hand example does need
         * to know what direction to point the necessary bridge in.
         *
         * Neither pass can handle the most general case, in which we
         * observe that an arbitrary subset of an island's neighbours
         * would form an isolated subgraph with it if it connected
         * maximally to them, and hence that at least one bridge must
         * point to some neighbour outside that subset but we don't
         * know which neighbour. To handle that, we'd have to have a
         * richer data format for the solver, which could cope with
         * recording the idea that at least one of two edges must have
         * a bridge.
         */
        int got = 0;
        int before[4];
        int j;

        spc = island_adjspace(is, 1, missing, i);
        if (spc == 0) continue;

        for (j = 0; j < is->adj.npoints; j++)
            before[j] = GRIDCOUNT(is->state,
                                  is->adj.points[j].x,
                                  is->adj.points[j].y,
                                  is->adj.points[j].dx ? G_LINEH : G_LINEV);
        if (before[i] != 0) continue;  /* this idea is pointless otherwise */

        memcpy(ss->tmpdsf, ss->dsf, wh*sizeof(int));

        for (j = 0; j < is->adj.npoints; j++) {
            spc = island_adjspace(is, 1, missing, j);
            if (spc == 0) continue;
            if (j == i) continue;
            solve_join(is, j, before[j] + spc, 0);
        }
        map_update_possibles(is->state);

        if (solve_island_subgroup(is, -1))
            got = 1;

        for (j = 0; j < is->adj.npoints; j++)
            solve_join(is, j, before[j], 0);
        memcpy(ss->dsf, ss->tmpdsf, wh*sizeof(int));

        if (got) {
            debug(("island at (%d,%d) must connect in direction (%d,%d) to"
                   " avoid full subgroup.\n",
                   is->x, is->y, is->adj.points[i].dx, is->adj.points[i].dy));
            solve_join(is, i, 1, 0);
            didsth = 1;
        }

        map_update_possibles(is->state);
    }

    if (didsth) *didsth_r = didsth;
    return 1;
}