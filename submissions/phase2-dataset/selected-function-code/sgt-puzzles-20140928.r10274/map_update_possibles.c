static void map_update_possibles(game_state *state)
{
    int x, y, s, e, bl, i, np, maxb, w = state->w, idx;
    struct island *is_s = NULL, *is_f = NULL;

    /* Run down vertical stripes [un]setting possv... */
    for (x = 0; x < state->w; x++) {
        idx = x;
        s = e = -1;
        bl = 0;
        maxb = state->params.maxb;     /* placate optimiser */
        /* Unset possible flags until we find an island. */
        for (y = 0; y < state->h; y++) {
            is_s = IDX(state, gridi, idx);
            if (is_s) {
                maxb = is_s->count;
                break;
            }

            IDX(state, possv, idx) = 0;
            idx += w;
        }
        for (; y < state->h; y++) {
            maxb = min(maxb, IDX(state, maxv, idx));
            is_f = IDX(state, gridi, idx);
            if (is_f) {
                assert(is_s);
                np = min(maxb, is_f->count);

                if (s != -1) {
                    for (i = s; i <= e; i++) {
                        INDEX(state, possv, x, i) = bl ? 0 : np;
                    }
                }
                s = y+1;
                bl = 0;
                is_s = is_f;
                maxb = is_s->count;
            } else {
                e = y;
                if (IDX(state,grid,idx) & (G_LINEH|G_NOLINEV)) bl = 1;
            }
            idx += w;
        }
        if (s != -1) {
            for (i = s; i <= e; i++)
                INDEX(state, possv, x, i) = 0;
        }
    }

    /* ...and now do horizontal stripes [un]setting possh. */
    /* can we lose this clone'n'hack? */
    for (y = 0; y < state->h; y++) {
        idx = y*w;
        s = e = -1;
        bl = 0;
        maxb = state->params.maxb;     /* placate optimiser */
        for (x = 0; x < state->w; x++) {
            is_s = IDX(state, gridi, idx);
            if (is_s) {
                maxb = is_s->count;
                break;
            }

            IDX(state, possh, idx) = 0;
            idx += 1;
        }
        for (; x < state->w; x++) {
            maxb = min(maxb, IDX(state, maxh, idx));
            is_f = IDX(state, gridi, idx);
            if (is_f) {
                assert(is_s);
                np = min(maxb, is_f->count);

                if (s != -1) {
                    for (i = s; i <= e; i++) {
                        INDEX(state, possh, i, y) = bl ? 0 : np;
                    }
                }
                s = x+1;
                bl = 0;
                is_s = is_f;
                maxb = is_s->count;
            } else {
                e = x;
                if (IDX(state,grid,idx) & (G_LINEV|G_NOLINEH)) bl = 1;
            }
            idx += 1;
        }
        if (s != -1) {
            for (i = s; i <= e; i++)
                INDEX(state, possh, i, y) = 0;
        }
    }
}