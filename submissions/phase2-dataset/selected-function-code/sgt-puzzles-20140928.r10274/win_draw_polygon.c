static void win_draw_polygon(void *handle, int *coords, int npoints,
			     int fillcolour, int outlinecolour)
{
    frontend *fe = (frontend *)handle;
    POINT *pts;
    int i;

    if (fe->drawstatus == NOTHING)
	return;

    pts = snewn(npoints+1, POINT);

    for (i = 0; i <= npoints; i++) {
	int j = (i < npoints ? i : 0);
	pts[i] = win_transform_point(fe, coords[j*2], coords[j*2+1]);
    }

    assert(outlinecolour >= 0);

    if (fillcolour >= 0) {
	win_set_brush(fe, fillcolour);
	win_set_pen(fe, outlinecolour, FALSE);
	Polygon(fe->hdc, pts, npoints);
	win_reset_brush(fe);
	win_reset_pen(fe);
    } else {
	win_set_pen(fe, outlinecolour, FALSE);
	Polyline(fe->hdc, pts, npoints+1);
	win_reset_pen(fe);
    }

    sfree(pts);
}