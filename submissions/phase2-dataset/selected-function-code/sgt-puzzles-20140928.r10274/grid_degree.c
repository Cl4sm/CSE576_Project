static int grid_degree(game_state *state, int x, int y, int *nx_r, int *ny_r)
{
    grid_type grid = SCRATCH(state, x, y), gline = grid & G_LINE;
    struct island *is;
    int x1, y1, x2, y2, c = 0, i, nx, ny;

    nx = ny = -1; /* placate optimiser */
    is = INDEX(state, gridi, x, y);
    if (is) {
        for (i = 0; i < is->adj.npoints; i++) {
            gline = is->adj.points[i].dx ? G_LINEH : G_LINEV;
            if (SCRATCH(state,
                        is->adj.points[i].x,
                        is->adj.points[i].y) & gline) {
                nx = is->adj.points[i].x;
                ny = is->adj.points[i].y;
                c++;
            }
        }
    } else if (gline) {
        if (gline & G_LINEV) {
            x1 = x2 = x;
            y1 = y-1; y2 = y+1;
        } else {
            x1 = x-1; x2 = x+1;
            y1 = y2 = y;
        }
        /* Non-island squares with edges in should never be pointing off the
         * edge of the grid. */
        assert(INGRID(state, x1, y1));
        assert(INGRID(state, x2, y2));
        if (SCRATCH(state, x1, y1) & (gline | G_ISLAND)) {
            nx = x1; ny = y1; c++;
        }
        if (SCRATCH(state, x2, y2) & (gline | G_ISLAND)) {
            nx = x2; ny = y2; c++;
        }
    }
    if (c == 1) {
        assert(nx != -1 && ny != -1); /* paranoia */
        *nx_r = nx; *ny_r = ny;
    }
    return c;
}