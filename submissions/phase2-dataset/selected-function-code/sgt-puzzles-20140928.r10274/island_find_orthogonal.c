static void island_find_orthogonal(struct island *is)
{
    /* fills in the rest of the 'surrounds' structure, assuming
     * all other islands are now in place. */
    int i, x, y, dx, dy, off;

    is->adj.nislands = 0;
    for (i = 0; i < is->adj.npoints; i++) {
        dx = is->adj.points[i].dx;
        dy = is->adj.points[i].dy;
        x = is->x + dx;
        y = is->y + dy;
        off = 1;
        is->adj.points[i].off = 0;
        while (INGRID(is->state, x, y)) {
            if (GRID(is->state, x, y) & G_ISLAND) {
                is->adj.points[i].off = off;
                is->adj.nislands++;
                /*debug(("island (%d,%d) has orth is. %d*(%d,%d) away at (%d,%d).\n",
                       is->x, is->y, off, dx, dy,
                       ISLAND_ORTHX(is,i), ISLAND_ORTHY(is,i)));*/
                goto foundisland;
            }
            off++; x += dx; y += dy;
        }
foundisland:
        ;
    }
}