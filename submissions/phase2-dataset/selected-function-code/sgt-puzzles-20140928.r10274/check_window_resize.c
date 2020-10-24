static int check_window_resize(frontend *fe, int cx, int cy,
                               int *px, int *py,
                               int *wx, int *wy)
{
    RECT r;
    int x, y, sy = get_statusbar_height(fe), changed = 0;

    /* disallow making window thinner than menu bar */
    x = max(cx, fe->xmin);
    y = max(cy - sy, fe->ymin);

    /*
     * See if we actually got the window size we wanted, and adjust
     * the puzzle size if not.
     */
    midend_size(fe->me, &x, &y, TRUE);
    if (x != cx || y != cy) {
        /*
         * Resize the window, now we know what size we _really_
         * want it to be.
         */
        r.left = r.top = 0;
        r.right = x;
        r.bottom = y + sy;
        AdjustWindowRectEx(&r, WINFLAGS, TRUE, 0);
        *wx = r.right - r.left;
        *wy = r.bottom - r.top;
        changed = 1;
    }

    *px = x;
    *py = y;

    fe->puzz_scale =
      (float)midend_tilesize(fe->me) / (float)fe->game->preferred_tilesize;

    return changed;
}