static int number_wrong(const game_state *state, int x, int y)
{
    surrounds s;
    int i, n, empty, lights = GRID(state, lights, x, y);

    /*
     * This function computes the display hint for a number: we
     * turn the number red if it is definitely wrong. This means
     * that either
     * 
     *  (a) it has too many lights around it, or
     * 	(b) it would have too few lights around it even if all the
     * 	    plausible squares (not black, lit or F_IMPOSSIBLE) were
     * 	    filled with lights.
     */

    assert(GRID(state, flags, x, y) & F_NUMBERED);
    get_surrounds(state, x, y, &s);

    empty = n = 0;
    for (i = 0; i < s.npoints; i++) {
	if (GRID(state,flags,s.points[i].x,s.points[i].y) & F_LIGHT) {
	    n++;
	    continue;
	}
	if (GRID(state,flags,s.points[i].x,s.points[i].y) & F_BLACK)
	    continue;
	if (GRID(state,flags,s.points[i].x,s.points[i].y) & F_IMPOSSIBLE)
	    continue;
	if (GRID(state,lights,s.points[i].x,s.points[i].y))
	    continue;
	empty++;
    }
    return (n > lights || (n + empty < lights));
}