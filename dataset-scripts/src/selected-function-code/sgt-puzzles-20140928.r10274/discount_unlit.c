static int discount_unlit(game_state *state, int x, int y,
                          struct setscratch *scratch)
{
    ll_data lld;
    int n, didsth;

#ifdef SOLVER_DIAGNOSTICS
    if (verbose) debug(("Trying to discount for unlit square at (%d,%d).\n", x, y));
    if (verbose > 1) debug_state(state);
#endif

    discount_clear(state, scratch, &n);

    list_lights(state, x, y, 1, &lld);
    FOREACHLIT(&lld, { unlit_cb(state, lx, ly, scratch, &n); });
    didsth = discount_set(state, scratch, n);
#ifdef SOLVER_DIAGNOSTICS
    if (didsth) debug(("  [from unlit square at (%d,%d)].\n", x, y));
#endif
    return didsth;

}