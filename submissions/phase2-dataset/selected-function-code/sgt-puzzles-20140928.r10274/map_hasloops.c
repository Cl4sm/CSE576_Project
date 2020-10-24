static int map_hasloops(game_state *state, int mark)
{
    int x, y, ox, oy, nx = 0, ny = 0, loop = 0;

    memcpy(state->scratch, state->grid, GRIDSZ(state));

    /* This algorithm is actually broken; if there are two loops connected
     * by bridges this will also highlight bridges. The correct algorithm
     * uses a dsf and a two-pass edge-detection algorithm (see check_correct
     * in slant.c); this is BALGE for now, especially since disallow-loops
     * is not the default for this puzzle. If we want to fix this later then
     * copy the alg in slant.c to the empty statement in map_group. */

    /* Remove all 1-degree edges. */
    for (y = 0; y < state->h; y++) {
        for (x = 0; x < state->w; x++) {
            ox = x; oy = y;
            while (grid_degree(state, ox, oy, &nx, &ny) == 1) {
                /*debug(("hasloops: removing 1-degree at (%d,%d).\n", ox, oy));*/
                SCRATCH(state, ox, oy) &= ~(G_LINE|G_ISLAND);
                ox = nx; oy = ny;
            }
        }
    }
    /* Mark any remaining edges as G_WARN, if required. */
    for (x = 0; x < state->w; x++) {
        for (y = 0; y < state->h; y++) {
            if (GRID(state,x,y) & G_ISLAND) continue;

            if (SCRATCH(state, x, y) & G_LINE) {
                if (mark) {
                    /*debug(("hasloops: marking loop square at (%d,%d).\n",
                           x, y));*/
                    GRID(state,x,y) |= G_WARN;
                    loop = 1;
                } else
                    return 1; /* short-cut as soon as we find one */
            } else {
                if (mark)
                    GRID(state,x,y) &= ~G_WARN;
            }
        }
    }
    return loop;
}