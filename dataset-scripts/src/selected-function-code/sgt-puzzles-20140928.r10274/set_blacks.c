static void set_blacks(game_state *state, const game_params *params,
                       random_state *rs)
{
    int x, y, degree = 0, rotate = 0, nblack;
    int rh, rw, i;
    int wodd = (state->w % 2) ? 1 : 0;
    int hodd = (state->h % 2) ? 1 : 0;
    int xs[4], ys[4];

    switch (params->symm) {
    case SYMM_NONE: degree = 1; rotate = 0; break;
    case SYMM_ROT2: degree = 2; rotate = 1; break;
    case SYMM_REF2: degree = 2; rotate = 0; break;
    case SYMM_ROT4: degree = 4; rotate = 1; break;
    case SYMM_REF4: degree = 4; rotate = 0; break;
    default: assert(!"Unknown symmetry type");
    }
    if (params->symm == SYMM_ROT4 && (state->h != state->w))
        assert(!"4-fold symmetry unavailable without square grid");

    if (degree == 4) {
        rw = state->w/2;
        rh = state->h/2;
        if (!rotate) rw += wodd; /* ... but see below. */
        rh += hodd;
    } else if (degree == 2) {
        rw = state->w;
        rh = state->h/2;
        rh += hodd;
    } else {
        rw = state->w;
        rh = state->h;
    }

    /* clear, then randomise, required region. */
    clean_board(state, 0);
    nblack = (rw * rh * params->blackpc) / 100;
    for (i = 0; i < nblack; i++) {
        do {
            x = random_upto(rs,rw);
            y = random_upto(rs,rh);
        } while (GRID(state,flags,x,y) & F_BLACK);
        GRID(state, flags, x, y) |= F_BLACK;
    }

    /* Copy required region. */
    if (params->symm == SYMM_NONE) return;

    for (x = 0; x < rw; x++) {
        for (y = 0; y < rh; y++) {
            if (degree == 4) {
                xs[0] = x;
                ys[0] = y;
                xs[1] = state->w - 1 - (rotate ? y : x);
                ys[1] = rotate ? x : y;
                xs[2] = rotate ? (state->w - 1 - x) : x;
                ys[2] = state->h - 1 - y;
                xs[3] = rotate ? y : (state->w - 1 - x);
                ys[3] = state->h - 1 - (rotate ? x : y);
            } else {
                xs[0] = x;
                ys[0] = y;
                xs[1] = rotate ? (state->w - 1 - x) : x;
                ys[1] = state->h - 1 - y;
            }
            for (i = 1; i < degree; i++) {
                GRID(state, flags, xs[i], ys[i]) =
                    GRID(state, flags, xs[0], ys[0]);
            }
        }
    }
    /* SYMM_ROT4 misses the middle square above; fix that here. */
    if (degree == 4 && rotate && wodd &&
        (random_upto(rs,100) <= (unsigned int)params->blackpc))
        GRID(state,flags,
             state->w/2 + wodd - 1, state->h/2 + hodd - 1) |= F_BLACK;

#ifdef SOLVER_DIAGNOSTICS
    if (verbose) debug_state(state);
#endif
}