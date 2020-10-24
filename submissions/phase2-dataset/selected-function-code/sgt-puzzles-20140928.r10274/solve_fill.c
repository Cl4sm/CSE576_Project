static int solve_fill(struct island *is)
{
    /* for each unmarked adjacent, make sure we convert every possible bridge
     * to a real one, and then work out the possibles afresh. */
    int i, nnew, ncurr, nadded = 0, missing;

    debug(("solve_fill for island (%d,%d).\n", is->x, is->y));

    missing = is->count - island_countbridges(is);
    if (missing < 0) return 0;

    /* very like island_countspaces. */
    for (i = 0; i < is->adj.npoints; i++) {
        nnew = island_adjspace(is, 1, missing, i);
        if (nnew) {
            ncurr = GRIDCOUNT(is->state,
                              is->adj.points[i].x, is->adj.points[i].y,
                              is->adj.points[i].dx ? G_LINEH : G_LINEV);

            solve_join(is, i, nnew + ncurr, 0);
            nadded += nnew;
        }
    }
    return nadded;
}