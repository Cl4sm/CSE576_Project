static char *game_state_diff(const game_state *src, const game_state *dest)
{
    int movesize = 256, movelen = 0;
    char *move = snewn(movesize, char), buf[80];
    int i, d, x, y, len;
    grid_type gline, nline;
    struct island *is_s, *is_d, *is_orth;

#define APPEND do {                                     \
    if (movelen + len >= movesize) {                    \
        movesize = movelen + len + 256;                 \
        move = sresize(move, movesize, char);           \
    }                                                   \
    strcpy(move + movelen, buf);                        \
    movelen += len;                                     \
} while(0)

    move[movelen++] = 'S';
    move[movelen] = '\0';

    assert(src->n_islands == dest->n_islands);

    for (i = 0; i < src->n_islands; i++) {
        is_s = &src->islands[i];
        is_d = &dest->islands[i];
        assert(is_s->x == is_d->x);
        assert(is_s->y == is_d->y);
        assert(is_s->adj.npoints == is_d->adj.npoints); /* more paranoia */

        for (d = 0; d < is_s->adj.npoints; d++) {
            if (is_s->adj.points[d].dx == -1 ||
                is_s->adj.points[d].dy == -1) continue;

            x = is_s->adj.points[d].x;
            y = is_s->adj.points[d].y;
            gline = is_s->adj.points[d].dx ? G_LINEH : G_LINEV;
            nline = is_s->adj.points[d].dx ? G_NOLINEH : G_NOLINEV;
            is_orth = INDEX(dest, gridi,
                            ISLAND_ORTHX(is_d, d), ISLAND_ORTHY(is_d, d));

            if (GRIDCOUNT(src, x, y, gline) != GRIDCOUNT(dest, x, y, gline)) {
                assert(is_orth);
                len = sprintf(buf, ";L%d,%d,%d,%d,%d",
                              is_s->x, is_s->y, is_orth->x, is_orth->y,
                              GRIDCOUNT(dest, x, y, gline));
                APPEND;
            }
            if ((GRID(src,x,y) & nline) != (GRID(dest, x, y) & nline)) {
                assert(is_orth);
                len = sprintf(buf, ";N%d,%d,%d,%d",
                              is_s->x, is_s->y, is_orth->x, is_orth->y);
                APPEND;
            }
        }
        if ((GRID(src, is_s->x, is_s->y) & G_MARK) !=
            (GRID(dest, is_d->x, is_d->y) & G_MARK)) {
            len = sprintf(buf, ";M%d,%d", is_s->x, is_s->y);
            APPEND;
        }
    }
    return move;
}