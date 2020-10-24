static gint configure_area(GtkWidget *widget,
                           GdkEventConfigure *event, gpointer data)
{
    frontend *fe = (frontend *)data;
    int x, y;

    if (backing_store_ok(fe))
	teardown_backing_store(fe);

    x = fe->w = event->width;
    y = fe->h = event->height;
    midend_size(fe->me, &x, &y, TRUE);
    fe->pw = x;
    fe->ph = y;
    fe->ox = (fe->w - fe->pw) / 2;
    fe->oy = (fe->h - fe->ph) / 2;

    setup_backing_store(fe);
    midend_force_redraw(fe->me);

    return TRUE;
}