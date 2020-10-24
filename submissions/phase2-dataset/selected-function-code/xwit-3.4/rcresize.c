static void
rcresize(enum functions what, Window window)
{
    XSizeHints *hints;
    long supplied;
    int w, h;

    if (!(what & FBIT(rows)) || !(what & FBIT(columns)))
	getsize(window, &w, &h);

    if (!(hints = XAllocSizeHints()))
	Fatal_Error("XAllocSizeHints");

    if (XGetWMNormalHints(dpy, window, hints, &supplied) == 0)
	Fatal_Error("XGetWMNormalHints");

    if (!(supplied & PBaseSize) || !(supplied & PResizeInc))
	Fatal_Error("missing PBaseSize and/or PResizeInc hint");

    if (what & FBIT(columns))
	w = hints->base_width + hints->width_inc * ncolumns;

    if (what & FBIT(rows))
	h = hints->base_height + hints->height_inc * nrows;

    doresize(window, w, h);

    XFree(hints);
}