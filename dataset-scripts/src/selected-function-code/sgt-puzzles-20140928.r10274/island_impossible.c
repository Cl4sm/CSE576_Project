static int island_impossible(struct island *is, int strict)
{
    int curr = island_countbridges(is), nspc = is->count - curr, nsurrspc;
    int i, poss;
    struct island *is_orth;

    if (nspc < 0) {
        debug(("island at (%d,%d) impossible because full.\n", is->x, is->y));
        return 1;        /* too many bridges */
    } else if ((curr + island_countspaces(is, 0)) < is->count) {
        debug(("island at (%d,%d) impossible because not enough spaces.\n", is->x, is->y));
        return 1;        /* impossible to create enough bridges */
    } else if (strict && curr < is->count) {
        debug(("island at (%d,%d) impossible because locked.\n", is->x, is->y));
        return 1;        /* not enough bridges and island is locked */
    }

    /* Count spaces in surrounding islands. */
    nsurrspc = 0;
    for (i = 0; i < is->adj.npoints; i++) {
        int ifree, dx = is->adj.points[i].dx;

        if (!is->adj.points[i].off) continue;
        poss = POSSIBLES(is->state, dx,
                         is->adj.points[i].x, is->adj.points[i].y);
        if (poss == 0) continue;
        is_orth = INDEX(is->state, gridi,
                        ISLAND_ORTHX(is,i), ISLAND_ORTHY(is,i));
        assert(is_orth);

        ifree = is_orth->count - island_countbridges(is_orth);
        if (ifree > 0) {
	    /*
	     * ifree is the number of bridges unfilled in the other
	     * island, which is clearly an upper bound on the number
	     * of extra bridges this island may run to it.
	     *
	     * Another upper bound is the number of bridges unfilled
	     * on the specific line between here and there. We must
	     * take the minimum of both.
	     */
	    int bmax = MAXIMUM(is->state, dx,
			       is->adj.points[i].x, is->adj.points[i].y);
	    int bcurr = GRIDCOUNT(is->state,
				  is->adj.points[i].x, is->adj.points[i].y,
				  dx ? G_LINEH : G_LINEV);
	    assert(bcurr <= bmax);
            nsurrspc += min(ifree, bmax - bcurr);
	}
    }
    if (nsurrspc < nspc) {
        debug(("island at (%d,%d) impossible: surr. islands %d spc, need %d.\n",
               is->x, is->y, nsurrspc, nspc));
        return 1;       /* not enough spaces around surrounding islands to fill this one. */
    }

    return 0;
}