static void island_join(struct island *i1, struct island *i2, int n, int is_max)
{
    game_state *state = i1->state;
    int s, e, x, y;

    assert(i1->state == i2->state);
    assert(n >= -1 && n <= i1->state->maxb);

    if (i1->x == i2->x) {
        x = i1->x;
        if (i1->y < i2->y) {
            s = i1->y+1; e = i2->y-1;
        } else {
            s = i2->y+1; e = i1->y-1;
        }
        for (y = s; y <= e; y++) {
            if (is_max) {
                INDEX(state,maxv,x,y) = n;
            } else {
                if (n < 0) {
                    GRID(state,x,y) ^= G_NOLINEV;
                } else if (n == 0) {
                    GRID(state,x,y) &= ~G_LINEV;
                } else {
                    GRID(state,x,y) |= G_LINEV;
                    INDEX(state,lines,x,y) = n;
                }
            }
        }
    } else if (i1->y == i2->y) {
        y = i1->y;
        if (i1->x < i2->x) {
            s = i1->x+1; e = i2->x-1;
        } else {
            s = i2->x+1; e = i1->x-1;
        }
        for (x = s; x <= e; x++) {
            if (is_max) {
                INDEX(state,maxh,x,y) = n;
            } else {
                if (n < 0) {
                    GRID(state,x,y) ^= G_NOLINEH;
                } else if (n == 0) {
                    GRID(state,x,y) &= ~G_LINEH;
                } else {
                    GRID(state,x,y) |= G_LINEH;
                    INDEX(state,lines,x,y) = n;
                }
            }
        }
    } else {
        assert(!"island_join: islands not orthogonal.");
    }
}