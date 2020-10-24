static int solve_sub(game_state *state,
                     unsigned int solve_flags, int depth,
                     int *maxdepth)
{
    unsigned int flags;
    int x, y, didstuff, ncanplace, lights;
    int bestx, besty, n, bestn, copy_soluble, self_soluble, ret, maxrecurse = 0;
    game_state *scopy;
    ll_data lld;
    struct setscratch *sscratch = NULL;

#ifdef SOLVER_DIAGNOSTICS
    printf("solve_sub: depth = %d\n", depth);
#endif
    if (maxdepth && *maxdepth < depth) *maxdepth = depth;
    if (solve_flags & F_SOLVE_ALLOWRECURSE) maxrecurse = MAXRECURSE;

    while (1) {
        if (grid_overlap(state)) {
            /* Our own solver, from scratch, should never cause this to happen
             * (assuming a soluble grid). However, if we're trying to solve
             * from a half-completed *incorrect* grid this might occur; we
             * just return the 'no solutions' code in this case. */
            ret = 0; goto done;
        }

        if (grid_correct(state)) { ret = 1; goto done; }

        ncanplace = 0;
        didstuff = 0;
        /* These 2 loops, and the functions they call, are the critical loops
         * for timing; any optimisations should look here first. */
        for (x = 0; x < state->w; x++) {
            for (y = 0; y < state->h; y++) {
                flags = GRID(state,flags,x,y);
                lights = GRID(state,lights,x,y);
                ncanplace += could_place_light(flags, lights);

                if (try_solve_light(state, x, y, flags, lights)) didstuff = 1;
                if (try_solve_number(state, x, y, flags, lights)) didstuff = 1;
            }
        }
        if (didstuff) continue;
        if (!ncanplace) {
            /* nowhere to put a light, puzzle is unsoluble. */
            ret = 0; goto done;
        }

        if (solve_flags & F_SOLVE_DISCOUNTSETS) {
            if (!sscratch) sscratch = snewn(SCRATCHSZ, struct setscratch);
            /* Try a more cunning (and more involved) way... more details above. */
            for (x = 0; x < state->w; x++) {
                for (y = 0; y < state->h; y++) {
                    flags = GRID(state,flags,x,y);
                    lights = GRID(state,lights,x,y);

                    if (!(flags & F_BLACK) && lights == 0) {
                        if (discount_unlit(state, x, y, sscratch)) {
                            didstuff = 1;
                            goto reduction_success;
                        }
                    } else if (flags & F_NUMBERED) {
                        if (discount_clue(state, x, y, sscratch)) {
                            didstuff = 1;
                            goto reduction_success;
                        }
                    }
                }
            }
        }
reduction_success:
        if (didstuff) continue;

        /* We now have to make a guess; we have places to put lights but
         * no definite idea about where they can go. */
        if (depth >= maxrecurse) {
            /* mustn't delve any deeper. */
            ret = -1; goto done;
        }
        /* Of all the squares that we could place a light, pick the one
         * that would light the most currently unlit squares. */
        /* This heuristic was just plucked from the air; there may well be
         * a more efficient way of choosing a square to flip to minimise
         * recursion. */
        bestn = 0;
        bestx = besty = -1; /* suyb */
        for (x = 0; x < state->w; x++) {
            for (y = 0; y < state->h; y++) {
                flags = GRID(state,flags,x,y);
                lights = GRID(state,lights,x,y);
                if (!could_place_light(flags, lights)) continue;

                n = 0;
                list_lights(state, x, y, 1, &lld);
                FOREACHLIT(&lld, { if (GRID(state,lights,lx,ly) == 0) n++; });
                if (n > bestn) {
                    bestn = n; bestx = x; besty = y;
                }
            }
        }
        assert(bestn > 0);
	assert(bestx >= 0 && besty >= 0);

        /* Now we've chosen a plausible (x,y), try to solve it once as 'lit'
         * and once as 'impossible'; we need to make one copy to do this. */

        scopy = dup_game(state);
#ifdef SOLVER_DIAGNOSTICS
        debug(("Recursing #1: trying (%d,%d) as IMPOSSIBLE\n", bestx, besty));
#endif
        GRID(state,flags,bestx,besty) |= F_IMPOSSIBLE;
        self_soluble = solve_sub(state, solve_flags,  depth+1, maxdepth);

        if (!(solve_flags & F_SOLVE_FORCEUNIQUE) && self_soluble > 0) {
            /* we didn't care about finding all solutions, and we just
             * found one; return with it immediately. */
            free_game(scopy);
            ret = self_soluble;
            goto done;
        }

#ifdef SOLVER_DIAGNOSTICS
        debug(("Recursing #2: trying (%d,%d) as LIGHT\n", bestx, besty));
#endif
        set_light(scopy, bestx, besty, 1);
        copy_soluble = solve_sub(scopy, solve_flags, depth+1, maxdepth);

        /* If we wanted a unique solution but we hit our recursion limit
         * (on either branch) then we have to assume we didn't find possible
         * extra solutions, and return 'not soluble'. */
        if ((solve_flags & F_SOLVE_FORCEUNIQUE) &&
            ((copy_soluble < 0) || (self_soluble < 0))) {
            ret = -1;
        /* Make sure that whether or not it was self or copy (or both) that
         * were soluble, that we return a solved state in self. */
        } else if (copy_soluble <= 0) {
            /* copy wasn't soluble; keep self state and return that result. */
            ret = self_soluble;
        } else if (self_soluble <= 0) {
            /* copy solved and we didn't, so copy in copy's (now solved)
             * flags and light state. */
            memcpy(state->lights, scopy->lights,
                   scopy->w * scopy->h * sizeof(int));
            memcpy(state->flags, scopy->flags,
                   scopy->w * scopy->h * sizeof(unsigned int));
            ret = copy_soluble;
        } else {
            ret = copy_soluble + self_soluble;
        }
        free_game(scopy);
        goto done;
    }
done:
    if (sscratch) sfree(sscratch);
#ifdef SOLVER_DIAGNOSTICS
    if (ret < 0)
        debug(("solve_sub: depth = %d returning, ran out of recursion.\n",
               depth));
    else
        debug(("solve_sub: depth = %d returning, %d solutions.\n",
               depth, ret));
#endif
    return ret;
}