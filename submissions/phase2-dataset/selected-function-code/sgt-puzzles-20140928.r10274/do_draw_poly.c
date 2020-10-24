static void do_draw_poly(frontend *fe, int *coords, int npoints,
			 int fillcolour, int outlinecolour)
{
    GdkPoint *points = snewn(npoints, GdkPoint);
    int i;

    for (i = 0; i < npoints; i++) {
        points[i].x = coords[i*2];
        points[i].y = coords[i*2+1];
    }

    if (fillcolour >= 0) {
	set_colour(fe, fillcolour);
	gdk_draw_polygon(fe->pixmap, fe->gc, TRUE, points, npoints);
    }
    assert(outlinecolour >= 0);
    set_colour(fe, outlinecolour);

    /*
     * In principle we ought to be able to use gdk_draw_polygon for
     * the outline as well. In fact, it turns out to interact badly
     * with a clipping region, for no terribly obvious reason, so I
     * draw the outline as a sequence of lines instead.
     */
    for (i = 0; i < npoints; i++)
	gdk_draw_line(fe->pixmap, fe->gc,
		      points[i].x, points[i].y,
		      points[(i+1)%npoints].x, points[(i+1)%npoints].y);

    sfree(points);
}