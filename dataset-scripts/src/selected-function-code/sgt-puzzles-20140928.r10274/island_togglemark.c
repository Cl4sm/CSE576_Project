static void island_togglemark(struct island *is)
{
    int i, j, x, y, o;
    struct island *is_loop;

    /* mark the island... */
    GRID(is->state, is->x, is->y) ^= G_MARK;

    /* ...remove all marks on non-island squares... */
    for (x = 0; x < is->state->w; x++) {
        for (y = 0; y < is->state->h; y++) {
            if (!(GRID(is->state, x, y) & G_ISLAND))
                GRID(is->state, x, y) &= ~G_MARK;
        }
    }

    /* ...and add marks to squares around marked islands. */
    for (i = 0; i < is->state->n_islands; i++) {
        is_loop = &is->state->islands[i];
        if (!(GRID(is_loop->state, is_loop->x, is_loop->y) & G_MARK))
            continue;

        for (j = 0; j < is_loop->adj.npoints; j++) {
            /* if this direction takes us to another island, mark all
             * squares between the two islands. */
            if (!is_loop->adj.points[j].off) continue;
            assert(is_loop->adj.points[j].off > 1);
            for (o = 1; o < is_loop->adj.points[j].off; o++) {
                GRID(is_loop->state,
                     is_loop->x + is_loop->adj.points[j].dx*o,
                     is_loop->y + is_loop->adj.points[j].dy*o) |=
                    is_loop->adj.points[j].dy ? G_MARKV : G_MARKH;
            }
        }
    }
}