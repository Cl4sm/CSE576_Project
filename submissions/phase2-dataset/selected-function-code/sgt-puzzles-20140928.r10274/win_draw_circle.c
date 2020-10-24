static void win_draw_circle(void *handle, int cx, int cy, int radius,
			    int fillcolour, int outlinecolour)
{
    frontend *fe = (frontend *)handle;
    POINT p, q;

    assert(outlinecolour >= 0);

    if (fe->drawstatus == NOTHING)
	return;

    if (fillcolour >= 0)
	win_set_brush(fe, fillcolour);
    else
	fe->oldbr = SelectObject(fe->hdc, GetStockObject(NULL_BRUSH));

    win_set_pen(fe, outlinecolour, FALSE);
    p = win_transform_point(fe, cx - radius, cy - radius);
    q = win_transform_point(fe, cx + radius, cy + radius);
    Ellipse(fe->hdc, p.x, p.y, q.x+1, q.y+1);
    win_reset_brush(fe);
    win_reset_pen(fe);
}