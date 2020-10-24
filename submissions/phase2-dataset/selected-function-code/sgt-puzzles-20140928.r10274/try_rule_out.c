static void try_rule_out(game_state *state, int x, int y,
                         struct setscratch *scratch, int n,
                         trl_cb cb, void *ctx)
{
    /* XXX Find all the squares which would rule out (x,y); anything
     * that would light it as well as squares adjacent to same clues
     * as X assuming that clue only has one remaining light.
     * Call the callback with each square. */
    ll_data lld;
    surrounds s, ss;
    int i, j, curr_lights, tot_lights;

    /* Find all squares that would rule out a light at (x,y) and call trl_cb
     * with them: anything that would light (x,y)... */

    list_lights(state, x, y, 0, &lld);
    FOREACHLIT(&lld, { if (could_place_light_xy(state, lx, ly)) { cb(state, lx, ly, scratch, n, ctx); } });

    /* ... as well as any empty space (that isn't x,y) next to any clue square
     * next to (x,y) that only has one light left to place. */

    get_surrounds(state, x, y, &s);
    for (i = 0; i < s.npoints; i++) {
        if (!(GRID(state,flags,s.points[i].x,s.points[i].y) & F_NUMBERED))
            continue;
        /* we have an adjacent clue square; find /its/ surrounds
         * and count the remaining lights it needs. */
        get_surrounds(state,s.points[i].x,s.points[i].y,&ss);
        curr_lights = 0;
        for (j = 0; j < ss.npoints; j++) {
            if (GRID(state,flags,ss.points[j].x,ss.points[j].y) & F_LIGHT)
                curr_lights++;
        }
        tot_lights = GRID(state, lights, s.points[i].x, s.points[i].y);
        /* We have a clue with tot_lights to fill, and curr_lights currently
         * around it. If adding a light at (x,y) fills up the clue (i.e.
         * curr_lights + 1 = tot_lights) then we need to discount all other
         * unlit squares around the clue. */
        if ((curr_lights + 1) == tot_lights) {
            for (j = 0; j < ss.npoints; j++) {
                int lx = ss.points[j].x, ly = ss.points[j].y;
                if (lx == x && ly == y) continue;
                if (could_place_light_xy(state, lx, ly))
                    cb(state, lx, ly, scratch, n, ctx);
            }
        }
    }
}