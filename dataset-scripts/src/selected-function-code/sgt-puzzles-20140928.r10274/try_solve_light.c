static int try_solve_light(game_state *state, int ox, int oy,
                           unsigned int flags, int lights)
{
    ll_data lld;
    int sx = 0, sy = 0, n = 0;

    if (lights > 0) return 0;
    if (flags & F_BLACK) return 0;

    /* We have an unlit square; count how many ways there are left to
     * place a light that lights us (including this square); if only
     * one, we must put a light there. Squares that could light us
     * are, of course, the same as the squares we would light... */
    list_lights(state, ox, oy, 1, &lld);
    FOREACHLIT(&lld, { tsl_callback(state, lx, ly, &sx, &sy, &n); });
    if (n == 1) {
        set_light(state, sx, sy, 1);
#ifdef SOLVER_DIAGNOSTICS
        debug(("(%d,%d) can only be lit from (%d,%d); setting to LIGHT\n",
                ox,oy,sx,sy));
        if (verbose) debug_state(state);
#endif
        return 1;
    }

    return 0;
}