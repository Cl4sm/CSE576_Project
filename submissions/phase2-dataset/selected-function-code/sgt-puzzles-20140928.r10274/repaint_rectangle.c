static void repaint_rectangle(frontend *fe, GtkWidget *widget,
			      int x, int y, int w, int h)
{
    GdkGC *gc = gdk_gc_new(widget->window);
#ifdef USE_CAIRO
    gdk_gc_set_foreground(gc, &fe->background);
#else
    gdk_gc_set_foreground(gc, &fe->colours[fe->backgroundindex]);
#endif
    if (x < fe->ox) {
	gdk_draw_rectangle(widget->window, gc,
			   TRUE, x, y, fe->ox - x, h);
	w -= (fe->ox - x);
	x = fe->ox;
    }
    if (y < fe->oy) {
	gdk_draw_rectangle(widget->window, gc,
			   TRUE, x, y, w, fe->oy - y);
	h -= (fe->oy - y);
	y = fe->oy;
    }
    if (w > fe->pw) {
	gdk_draw_rectangle(widget->window, gc,
			   TRUE, x + fe->pw, y, w - fe->pw, h);
	w = fe->pw;
    }
    if (h > fe->ph) {
	gdk_draw_rectangle(widget->window, gc,
			   TRUE, x, y + fe->ph, w, h - fe->ph);
	h = fe->ph;
    }
    gdk_draw_pixmap(widget->window, gc, fe->pixmap,
		    x - fe->ox, y - fe->oy, x, y, w, h);
    gdk_gc_unref(gc);
}