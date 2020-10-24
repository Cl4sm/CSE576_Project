static int island_isadj(struct island *is, int direction)
{
    int x, y;
    grid_type gline, mline;

    x = is->adj.points[direction].x;
    y = is->adj.points[direction].y;

    mline = is->adj.points[direction].dx ? G_MARKH : G_MARKV;
    gline = is->adj.points[direction].dx ? G_LINEH : G_LINEV;
    if (GRID(is->state, x, y) & mline) {
        /* If we're marked (i.e. the thing to attach to is complete)
         * only count an adjacency if we're already attached. */
        return GRIDCOUNT(is->state, x, y, gline);
    } else {
        /* If we're unmarked, count possible adjacency iff it's
         * flagged as POSSIBLE. */
        return POSSIBLES(is->state, is->adj.points[direction].dx, x, y);
    }
    return 0;
}