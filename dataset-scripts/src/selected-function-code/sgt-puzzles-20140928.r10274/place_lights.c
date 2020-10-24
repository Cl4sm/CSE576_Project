static void place_lights(game_state *state, random_state *rs)
{
    int i, x, y, n, *numindices, wh = state->w*state->h;
    ll_data lld;

    numindices = snewn(wh, int);
    for (i = 0; i < wh; i++) numindices[i] = i;
    shuffle(numindices, wh, sizeof(*numindices), rs);

    /* Place a light on all grid squares without lights. */
    for (x = 0; x < state->w; x++) {
        for (y = 0; y < state->h; y++) {
            GRID(state, flags, x, y) &= ~F_MARK; /* we use this later. */
            if (GRID(state, flags, x, y) & F_BLACK) continue;
            set_light(state, x, y, 1);
        }
    }

    for (i = 0; i < wh; i++) {
        y = numindices[i] / state->w;
        x = numindices[i] % state->w;
        if (!(GRID(state, flags, x, y) & F_LIGHT)) continue;
        if (GRID(state, flags, x, y) & F_MARK) continue;
        list_lights(state, x, y, 0, &lld);

        /* If we're not lighting any lights ourself, don't remove anything. */
        n = 0;
        FOREACHLIT(&lld, if (GRID(state,flags,lx,ly) & F_LIGHT) { n += 1; } );
        if (n == 0) continue; /* [1] */

        /* Check whether removing lights we're lighting would cause anything
         * to go dark. */
        n = 0;
        FOREACHLIT(&lld, if (GRID(state,flags,lx,ly) & F_LIGHT) { n += check_dark(state,lx,ly); } );
        if (n == 0) {
            /* No, it wouldn't, so we can remove them all. */
            FOREACHLIT(&lld, set_light(state,lx,ly, 0); );
            GRID(state,flags,x,y) |= F_MARK;
        }

        if (!grid_overlap(state)) {
            sfree(numindices);
            return; /* we're done. */
        }
        assert(grid_lit(state));
    }
    /* could get here if the line at [1] continue'd out of the loop. */
    if (grid_overlap(state)) {
        debug_state(state);
        assert(!"place_lights failed to resolve overlapping lights!");
    }
    sfree(numindices);
}