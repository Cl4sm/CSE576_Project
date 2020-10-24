static void trl_callback_discount(game_state *state, int dx, int dy,
                       struct setscratch *scratch, int n, void *ctx)
{
    int *didsth = (int *)ctx;
    int i;

    if (GRID(state,flags,dx,dy) & F_IMPOSSIBLE) {
#ifdef SOLVER_DIAGNOSTICS
        debug(("Square at (%d,%d) already impossible.\n", dx,dy));
#endif
        return;
    }

    /* Check whether a light at (dx,dy) rules out everything
     * in scratch, and mark (dx,dy) as IMPOSSIBLE if it does.
     * We can use try_rule_out for this as well, as the set of
     * squares which would rule out (x,y) is the same as the
     * set of squares which (x,y) would rule out. */

#ifdef SOLVER_DIAGNOSTICS
    if (verbose) debug(("Checking whether light at (%d,%d) rules out everything in scratch.\n", dx, dy));
#endif

    for (i = 0; i < n; i++)
        scratch[i].n = 0;
    try_rule_out(state, dx, dy, scratch, n, trl_callback_search, NULL);
    for (i = 0; i < n; i++) {
        if (scratch[i].n == 0) return;
    }
    /* The light ruled out everything in scratch. Yay. */
    GRID(state,flags,dx,dy) |= F_IMPOSSIBLE;
#ifdef SOLVER_DIAGNOSTICS
    debug(("Set reduction discounted square at (%d,%d):\n", dx,dy));
    if (verbose) debug_state(state);
#endif

    *didsth = 1;
}