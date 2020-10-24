static void
doresize(Window window, int w, int h)
{
    XWindowChanges values;
    unsigned int value_mask;
    int try;
    int nw, nh;

    values.width = w;
    values.height = h;
    value_mask = CWWidth | CWHeight;

    for (try = 0; try < 2; try++) {
	if (XReconfigureWMWindow(dpy, window, screen, value_mask, &values) == 0)
	    Fatal_Error("resize: XReconfigureWMWindow");

	getsize(window, &nw, &nh);
	if (values.width == nw && values.height == nh)
	    return;

	/* give window manager a couple of chances to react */
	mssleep(100);
	getsize(window, &nw, &nh);
	if (values.width == nw && values.height == nh)
	    return;

	mssleep(400);
	getsize(window, &nw, &nh);
	if (values.width == nw && values.height == nh)
	    return;
    }

    /* last chance */
    values.width += values.width - nw;
    values.height += values.height - nh;
    if (XReconfigureWMWindow(dpy, window, screen, value_mask, &values) == 0)
	Fatal_Error("resize: XReconfigureWMWindow 2");
}