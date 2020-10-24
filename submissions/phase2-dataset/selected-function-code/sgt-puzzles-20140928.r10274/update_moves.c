static void update_moves(unsigned char *grid, int w, int h, int x, int y,
			 struct movetrees *trees)
{
    struct move move;
    int dir, pos;

    /*
     * There are twelve moves that can include (x,y): three in each
     * of four directions. Check each one to see if it's possible.
     */
    for (dir = 0; dir < 4; dir++) {
	int dx, dy;

	if (dir & 1)
	    dx = 0, dy = dir - 2;
	else
	    dy = 0, dx = dir - 1;

	assert(abs(dx) + abs(dy) == 1);

	for (pos = 0; pos < 3; pos++) {
	    int v1, v2, v3;

	    move.dx = dx;
	    move.dy = dy;
	    move.x = x - pos*dx;
	    move.y = y - pos*dy;

	    if (move.x < 0 || move.x >= w || move.y < 0 || move.y >= h)
		continue;	       /* completely invalid move */
	    if (move.x+2*move.dx < 0 || move.x+2*move.dx >= w ||
		move.y+2*move.dy < 0 || move.y+2*move.dy >= h)
		continue;	       /* completely invalid move */

	    v1 = grid[move.y * w + move.x];
	    v2 = grid[(move.y+move.dy) * w + (move.x+move.dx)];
	    v3 = grid[(move.y+2*move.dy)*w + (move.x+2*move.dx)];
	    if (v1 == GRID_PEG && v2 != GRID_PEG && v3 != GRID_PEG) {
		struct move *m;

		move.cost = (v2 == GRID_OBST) + (v3 == GRID_OBST);

		/*
		 * This move is possible. See if it's already in
		 * the tree.
		 */
		m = find234(trees->bymove, &move, NULL);
		if (m && m->cost != move.cost) {
		    /*
		     * It's in the tree but listed with the wrong
		     * cost. Remove the old version.
		     */
#ifdef GENERATION_DIAGNOSTICS
		    printf("correcting %d%+d,%d%+d at cost %d\n",
			   m->x, m->dx, m->y, m->dy, m->cost);
#endif
		    del234(trees->bymove, m);
		    del234(trees->bycost, m);
		    sfree(m);
		    m = NULL;
		}
		if (!m) {
		    struct move *m, *m2;
		    m = snew(struct move);
		    *m = move;
		    m2 = add234(trees->bymove, m);
		    m2 = add234(trees->bycost, m);
		    assert(m2 == m);
#ifdef GENERATION_DIAGNOSTICS
		    printf("adding %d%+d,%d%+d at cost %d\n",
			   move.x, move.dx, move.y, move.dy, move.cost);
#endif
		} else {
#ifdef GENERATION_DIAGNOSTICS
		    printf("not adding %d%+d,%d%+d at cost %d\n",
			   move.x, move.dx, move.y, move.dy, move.cost);
#endif
		}
	    } else {
		/*
		 * This move is impossible. If it is already in the
		 * tree, delete it.
		 * 
		 * (We make use here of the fact that del234
		 * doesn't have to be passed a pointer to the
		 * _actual_ element it's deleting: it merely needs
		 * one that compares equal to it, and it will
		 * return the one it deletes.)
		 */
		struct move *m = del234(trees->bymove, &move);
#ifdef GENERATION_DIAGNOSTICS
		printf("%sdeleting %d%+d,%d%+d\n", m ? "" : "not ",
		       move.x, move.dx, move.y, move.dy);
#endif
		if (m) {
		    del234(trees->bycost, m);
		    sfree(m);
		}
	    }
	}
    }
}