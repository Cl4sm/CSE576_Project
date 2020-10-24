static int solve_island_stage1(struct island *is, int *didsth_r)
{
    int bridges = island_countbridges(is);
    int nspaces = island_countspaces(is, 1);
    int nadj = island_countadj(is);
    int didsth = 0;

    assert(didsth_r);

    /*debug(("island at (%d,%d) filled %d/%d (%d spc) nadj %d\n",
           is->x, is->y, bridges, is->count, nspaces, nadj));*/
    if (bridges > is->count) {
        /* We only ever add bridges when we're sure they fit, or that's
         * the only place they can go. If we've added bridges such that
         * another island has become wrong, the puzzle must not have had
         * a solution. */
        debug(("...island at (%d,%d) is overpopulated!\n", is->x, is->y));
        return 0;
    } else if (bridges == is->count) {
        /* This island is full. Make sure it's marked (and update
         * possibles if we did). */
        if (!(GRID(is->state, is->x, is->y) & G_MARK)) {
            debug(("...marking island (%d,%d) as full.\n", is->x, is->y));
            island_togglemark(is);
            didsth = 1;
        }
    } else if (GRID(is->state, is->x, is->y) & G_MARK) {
        debug(("...island (%d,%d) is marked but unfinished!\n",
               is->x, is->y));
        return 0; /* island has been marked unfinished; no solution from here. */
    } else {
        /* This is the interesting bit; we try and fill in more information
         * about this island. */
        if (is->count == bridges + nspaces) {
            if (solve_fill(is) > 0) didsth = 1;
        } else if (is->count > ((nadj-1) * is->state->maxb)) {
            /* must have at least one bridge in each possible direction. */
            if (solve_fillone(is) > 0) didsth = 1;
        }
    }
    if (didsth) {
        map_update_possibles(is->state);
        *didsth_r = 1;
    }
    return 1;
}