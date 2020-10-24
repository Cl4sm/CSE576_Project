static int try_solve_number(game_state *state, int nx, int ny,
                            unsigned int nflags, int nlights)
{
    surrounds s;
    int x, y, nl, ns, i, ret = 0, lights;
    unsigned int flags;

    if (!(nflags & F_NUMBERED)) return 0;
    nl = nlights;
    get_surrounds(state,nx,ny,&s);
    ns = s.npoints;

    /* nl is no. of lights we need to place, ns is no. of spaces we
     * have to place them in. Try and narrow these down, and mark
     * points we can ignore later. */
    for (i = 0; i < s.npoints; i++) {
        x = s.points[i].x; y = s.points[i].y;
        flags = GRID(state,flags,x,y);
        lights = GRID(state,lights,x,y);
        if (flags & F_LIGHT) {
            /* light here already; one less light for one less place. */
            nl--; ns--;
            s.points[i].f |= F_MARK;
        } else if (!could_place_light(flags, lights)) {
            ns--;
            s.points[i].f |= F_MARK;
        }
    }
    if (ns == 0) return 0; /* nowhere to put anything. */
    if (nl == 0) {
        /* we have placed all lights we need to around here; all remaining
         * surrounds are therefore IMPOSSIBLE. */
        GRID(state,flags,nx,ny) |= F_NUMBERUSED;
        for (i = 0; i < s.npoints; i++) {
            if (!(s.points[i].f & F_MARK)) {
                GRID(state,flags,s.points[i].x,s.points[i].y) |= F_IMPOSSIBLE;
                ret = 1;
            }
        }
#ifdef SOLVER_DIAGNOSTICS
        printf("Clue at (%d,%d) full; setting unlit to IMPOSSIBLE.\n",
               nx,ny);
        if (verbose) debug_state(state);
#endif
    } else if (nl == ns) {
        /* we have as many lights to place as spaces; fill them all. */
        GRID(state,flags,nx,ny) |= F_NUMBERUSED;
        for (i = 0; i < s.npoints; i++) {
            if (!(s.points[i].f & F_MARK)) {
                set_light(state, s.points[i].x,s.points[i].y, 1);
                ret = 1;
            }
        }
#ifdef SOLVER_DIAGNOSTICS
        printf("Clue at (%d,%d) trivial; setting unlit to LIGHT.\n",
               nx,ny);
        if (verbose) debug_state(state);
#endif
    }
    return ret;
}