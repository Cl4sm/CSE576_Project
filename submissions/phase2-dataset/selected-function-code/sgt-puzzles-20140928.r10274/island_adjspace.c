static int island_adjspace(struct island *is, int marks, int missing,
                           int direction)
{
    int x, y, poss, curr, dx;
    grid_type gline, mline;

    x = is->adj.points[direction].x;
    y = is->adj.points[direction].y;
    dx = is->adj.points[direction].dx;
    gline = dx ? G_LINEH : G_LINEV;

    if (marks) {
        mline = dx ? G_MARKH : G_MARKV;
        if (GRID(is->state,x,y) & mline) return 0;
    }
    poss = POSSIBLES(is->state, dx, x, y);
    poss = min(poss, missing);

    curr = GRIDCOUNT(is->state, x, y, gline);
    poss = min(poss, MAXIMUM(is->state, dx, x, y) - curr);

    return poss;
}