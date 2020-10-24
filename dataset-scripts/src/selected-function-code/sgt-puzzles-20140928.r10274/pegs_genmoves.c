static void pegs_genmoves(unsigned char *grid, int w, int h, random_state *rs)
{
    struct movetrees atrees, *trees = &atrees;
    struct move *m;
    int x, y, i, nmoves;

    trees->bymove = newtree234(movecmp);
    trees->bycost = newtree234(movecmpcost);

    for (y = 0; y < h; y++)
	for (x = 0; x < w; x++)
	    if (grid[y*w+x] == GRID_PEG)
		update_moves(grid, w, h, x, y, trees);

    nmoves = 0;

    while (1) {
	int limit, maxcost, index;
	struct move mtmp, move, *m;

	/*
	 * See how many moves we can make at zero cost. Make one,
	 * if possible. Failing that, make a one-cost move, and
	 * then a two-cost one.
	 * 
	 * After filling at least half the input grid, we no longer
	 * accept cost-2 moves: if that's our only option, we give
	 * up and finish.
	 */
	mtmp.y = h+1;
	maxcost = (nmoves < w*h/2 ? 2 : 1);
	m = NULL;		       /* placate optimiser */
	for (mtmp.cost = 0; mtmp.cost <= maxcost; mtmp.cost++) {
	    limit = -1;
	    m = findrelpos234(trees->bycost, &mtmp, NULL, REL234_LT, &limit);
#ifdef GENERATION_DIAGNOSTICS
	    printf("%d moves available with cost %d\n", limit+1, mtmp.cost);
#endif
	    if (m)
		break;
	}
	if (!m)
	    break;

	index = random_upto(rs, limit+1);
	move = *(struct move *)index234(trees->bycost, index);

#ifdef GENERATION_DIAGNOSTICS
	printf("selecting move %d%+d,%d%+d at cost %d\n",
	       move.x, move.dx, move.y, move.dy, move.cost);
#endif

	grid[move.y * w + move.x] = GRID_HOLE;
	grid[(move.y+move.dy) * w + (move.x+move.dx)] = GRID_PEG;
	grid[(move.y+2*move.dy)*w + (move.x+2*move.dx)] = GRID_PEG;

	for (i = 0; i <= 2; i++) {
	    int tx = move.x + i*move.dx;
	    int ty = move.y + i*move.dy;
	    update_moves(grid, w, h, tx, ty, trees);
	}

	nmoves++;
    }

    while ((m = delpos234(trees->bymove, 0)) != NULL) {
	del234(trees->bycost, m);
	sfree(m);
    }
    freetree234(trees->bymove);
    freetree234(trees->bycost);
}