static int discount_set(game_state *state,
                        struct setscratch *scratch, int n)
{
    int i, besti, bestn, didsth = 0;

#ifdef SOLVER_DIAGNOSTICS
    if (verbose > 1) debug_scratch("discount_set", scratch, n);
#endif
    if (n == 0) return 0;

    for (i = 0; i < n; i++) {
        try_rule_out(state, scratch[i].x, scratch[i].y, scratch, n,
                     trl_callback_incn, (void*)&(scratch[i]));
    }
#ifdef SOLVER_DIAGNOSTICS
    if (verbose > 1) debug_scratch("discount_set after count", scratch, n);
#endif

    besti = -1; bestn = SCRATCHSZ;
    for (i = 0; i < n; i++) {
        if (scratch[i].n < bestn) {
            bestn = scratch[i].n;
            besti = i;
        }
    }
#ifdef SOLVER_DIAGNOSTICS
    if (verbose > 1) debug(("best square (%d,%d) with n%d.\n",
           scratch[besti].x, scratch[besti].y, scratch[besti].n));
#endif
    try_rule_out(state, scratch[besti].x, scratch[besti].y, scratch, n,
                 trl_callback_discount, (void*)&didsth);
#ifdef SOLVER_DIAGNOSTICS
    if (didsth) debug((" [from square (%d,%d)]\n",
                       scratch[besti].x, scratch[besti].y));
#endif

    return didsth;
}