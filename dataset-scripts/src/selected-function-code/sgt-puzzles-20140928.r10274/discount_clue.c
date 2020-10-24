static int discount_clue(game_state *state, int x, int y,
                          struct setscratch *scratch)
{
    int slen, m = GRID(state, lights, x, y), n, i, didsth = 0, lights;
    unsigned int flags;
    surrounds s, sempty;
    combi_ctx *combi;

    if (m == 0) return 0;

#ifdef SOLVER_DIAGNOSTICS
    if (verbose) debug(("Trying to discount for sets at clue (%d,%d).\n", x, y));
    if (verbose > 1) debug_state(state);
#endif

    /* m is no. of lights still to place; starts off at the clue value
     * and decreases when we find a light already down.
     * n is no. of spaces left; starts off at 0 and goes up when we find
     * a plausible space. */

    get_surrounds(state, x, y, &s);
    memset(&sempty, 0, sizeof(surrounds));
    for (i = 0; i < s.npoints; i++) {
        int lx = s.points[i].x, ly = s.points[i].y;
        flags = GRID(state,flags,lx,ly);
        lights = GRID(state,lights,lx,ly);

        if (flags & F_LIGHT) m--;

        if (could_place_light(flags, lights)) {
            sempty.points[sempty.npoints].x = lx;
            sempty.points[sempty.npoints].y = ly;
            sempty.npoints++;
        }
    }
    n = sempty.npoints; /* sempty is now a surrounds of only blank squares. */
    if (n == 0) return 0; /* clue is full already. */

    if (m < 0 || m > n) return 0; /* become impossible. */

    combi = new_combi(n - m + 1, n);
    while (next_combi(combi)) {
        discount_clear(state, scratch, &slen);
        for (i = 0; i < combi->r; i++) {
            scratch[slen].x = sempty.points[combi->a[i]].x;
            scratch[slen].y = sempty.points[combi->a[i]].y;
            slen++;
        }
        if (discount_set(state, scratch, slen)) didsth = 1;
    }
    free_combi(combi);
#ifdef SOLVER_DIAGNOSTICS
    if (didsth) debug(("  [from clue at (%d,%d)].\n", x, y));
#endif
    return didsth;
}